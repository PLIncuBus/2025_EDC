# -----------------------------------------------------------------------------
# main_merged_vision_controller.py
#
# 功能描述:
# 1. 集成灰度(Grayscale)和LAB颜色空间两种阈值调试模式。
# 2. 支持在两种调试模式间动态切换。
# 3. 支持将两种阈值配置保存到SD卡，并在启动时自动加载。*
from media.display import Display
from media.media import MediaManager
from machine import TOUCH, UART, FPIOA
import image 
import time, os, sys
from media.sensor import Sensor


# --- 1. UART 通信类 ---
class Uart_Commend:
    """处理UART通信，包括数据打包和发送。"""
    def __init__(self):
        """初始化UART2，设置引脚和通信参数。"""
        self.fpioa = FPIOA()
        self.fpioa.set_function(11, FPIOA.UART2_TXD)
        self.fpioa.set_function(12, FPIOA.UART2_RXD)
        self.uart = UART(UART.UART2, baudrate=115200, bits=UART.EIGHTBITS, parity=UART.PARITY_NONE, stop=UART.STOPBITS_ONE)
        #发送数据包
        self.head = "@"
        self.tail1 = "#"
        self.tail2 = "$"
        
        #接收数据包
        self.head_task="!"
        self.tail1_task="&"
        self.tail2_task="*"

        self.uart_package=""

        print("UART2 初始化成功，波特率 115200。")

    def uart_send(self, message):
        """将消息打包并发送。"""
        self.uart.write(self.head + message + self.tail1 + self.tail2)
    def uart_rece(self):
        data = self.uart.read()
        if data:
            if len(data) >= 4 and data[0] == "!" and data[-2:] == "&*":
                return data[1:-2]  # 去掉帧头和帧尾
        return ""
    
    


# --- 2. SD卡配置管理器 ---
class ConfigManager:
    """通过在SD卡上读写文件来管理配置。"""
    CONFIG_DIR = "/sdcard/config"
    CONFIG_FILE = CONFIG_DIR + "/threshold_config.txt"

    def __init__(self):
        try:
            os.stat(self.CONFIG_DIR)
        except OSError:
            print(f"配置目录 {self.CONFIG_DIR} 不存在，正在创建...")
            try:
                os.mkdir(self.CONFIG_DIR)
            except Exception as e:
                print(f"创建目录失败: {e}")

    def save_config(self, thresholds_dict):
        """将包含所有阈值的字典保存到SD卡。"""
        try:
            with open(self.CONFIG_FILE, "w") as f:
                for key, values in thresholds_dict.items():
                    if values:
                        value_str = ",".join(map(str, values[0]))
                        f.write(f"{key}:{value_str}\n")
            print(f"配置已成功保存到: {self.CONFIG_FILE}")
            return True
        except Exception as e:
            print(f"保存配置到SD卡失败: {e}")
            return False

    def load_config(self):
        """从SD卡加载配置，返回一个包含所有阈值的字典。"""
        loaded_thresholds = {}
        try:
            with open(self.CONFIG_FILE, "r") as f:
                for line in f:
                    line = line.strip()
                    if not line or ':' not in line:
                        continue
                    key, value_str = line.split(':', 1)
                    values = tuple(map(int, value_str.split(',')))
                    loaded_thresholds[key] = [values]
            if loaded_thresholds:
                print(f"从 {self.CONFIG_FILE} 成功加载配置。")
            return loaded_thresholds
        except (OSError, ValueError) as e:
            print(f"从SD卡加载配置失败或文件不存在: {e}")
            return {}

# --- 3. 配置类 ---
class Config:
    SENSOR_WIDTH, SENSOR_HEIGHT = 640, 480
    LCD_WIDTH, LCD_HEIGHT = 800, 480
    ROI = ((SENSOR_WIDTH - 480) // 2, (SENSOR_HEIGHT - 480) // 2, 480, 480)

    BG_COLOR, PANEL_BG_COLOR = (240, 240, 240), (220, 220, 220)
    SLIDER_BG_COLOR, SLIDER_FG_COLOR = (200, 200, 200), (100, 100, 255)
    BUTTON_COLOR, TEXT_COLOR = (180, 180, 180), (0, 0, 0)
    HIGHLIGHT_COLOR, RECT_COLOR, CORNER_COLOR = (0, 255, 0), (0, 255, 0), (255, 0, 0)
    WHITE_COLOR = (255, 255, 255) # 新增白色，用于准星效果

    MODE_NORMAL, MODE_TUNING = 0, 1
    TOUCH_HOLD_FRAMES = 20
    RECT_THRESHOLD = 10000
    BLOB_PIXELS_THRESHOLD = 5 # 最小光斑面积
    BLOB_MERGE_DISTANCE = 3   # 合并光斑的距离

    DEFAULT_GRAY_THRESHOLD = (0, 80)
    DEFAULT_LAB_THRESHOLD = (30, 100, -100, -30, 0, 90)

# --- 4. UI 控件类 ---
class SliderWidget:
    def __init__(self, x, y, width, height, label, min_val, max_val, current_val):
        self.rect = (x, y, width, height)
        self.label = label
        self.min_val = min_val
        self.max_val = max_val
        self.value = current_val
        self.btn_minus_rect = (x, y, 60, height)
        self.btn_plus_rect = (x + width - 60, y, 60, height)
        self.slider_bar_rect = (x + 70, y + height // 2 - 5, width - 140, 10)

    def draw(self, img):
        img.draw_rectangle(*self.btn_minus_rect, color=Config.BUTTON_COLOR, fill=True)
        img.draw_string_advanced(self.rect[0] + 20, self.rect[1] + 10, 30, "-", color=Config.TEXT_COLOR)
        img.draw_rectangle(*self.btn_plus_rect, color=Config.BUTTON_COLOR, fill=True)
        img.draw_string_advanced(self.rect[0] + self.rect[2] - 40, self.rect[1] + 10, 30, "+", color=Config.TEXT_COLOR)
        bar_x, bar_y, bar_w, bar_h = self.slider_bar_rect
        img.draw_rectangle(bar_x, bar_y, bar_w, bar_h, color=Config.SLIDER_BG_COLOR, fill=True)
        value_range = self.max_val - self.min_val
        fill_w = int(((self.value - self.min_val) / value_range) * bar_w) if value_range != 0 else 0
        if fill_w > 0:
            img.draw_rectangle(bar_x, bar_y, fill_w, bar_h, color=Config.SLIDER_FG_COLOR, fill=True)
        label_text = f"{self.label}: {self.value}"
        img.draw_string_advanced(self.rect[0] + 70, self.rect[1] - 5, 25, label_text, color=Config.TEXT_COLOR)

    def check_touch(self, x, y):
        if self.rect[1] < y < self.rect[1] + self.rect[3]:
            if self.btn_minus_rect[0] < x < self.btn_minus_rect[0] + self.btn_minus_rect[2]: return -2
            if self.btn_plus_rect[0] < x < self.btn_plus_rect[0] + self.btn_plus_rect[2]: return 2
        return 0

    def update_value(self, change):
        self.value += change
        self.value = max(self.min_val, min(self.max_val, self.value))

# --- 5. 全局变量与初始化 ---
sensor = None
config_manager = ConfigManager()
uart_comm = Uart_Commend()
state = {
    'mode': Config.MODE_NORMAL,
    'touch_counter': 0,
    'thresholds': {
        'grayscale': [Config.DEFAULT_GRAY_THRESHOLD],
        'color': [Config.DEFAULT_LAB_THRESHOLD]
    },
    'last_frame_time': time.ticks_ms(),
    'fps_counter': 0,
    'fps': 0
}

# --- 6. 状态处理函数 ---

def handle_normal_mode():
    """【已修改】混合模式：同时寻找灰度矩形和LAB色块，并统一发送坐标。"""
    global state
    current_time = time.ticks_ms()
    # img = sensor.snapshot().copy(roi=Config.ROI)
    img = sensor.snapshot().copy()
    all_coords_flat = []
    
    # --- 1. 灰度矩形检测 ---
    rect_threshold = state['thresholds']['grayscale'][0]
    img_rect_proc = img.to_grayscale(copy=True)
    img_rect_proc.binary([rect_threshold])
    rects = img_rect_proc.find_rects(threshold=Config.RECT_THRESHOLD)
    rects.sort(key=lambda r: r.w() * r.h(), reverse=True)
    
    # 处理找到的矩形，最多两个
    for i in range(2):
        if i < len(rects):
            rect = rects[i]
            corners = rect.corners()
            # 中心点坐标 = 四角平均值（更稳定）
            center_x = sum(p[0] for p in corners) // 4
            center_y = sum(p[1] for p in corners) // 4
            # 绘制中心点
            img.draw_circle(center_x, center_y, 10, color=Config.RECT_COLOR, thickness=2, fill=False)
            # 添加中心点坐标
            all_coords_flat.extend([center_x, center_y])
        else:
            # 没有矩形，用0补齐
            all_coords_flat.extend([0, 0])



    # --- 2. LAB色块检测 ---
    lab_threshold = state['thresholds']['color'][0]
    blobs = img.find_blobs([lab_threshold], pixels_threshold=Config.BLOB_PIXELS_THRESHOLD, merge=True, margin=Config.BLOB_MERGE_DISTANCE)
    
    if blobs:
        # 找到最大的色块
        biggest_blob = max(blobs, key=lambda b: b.pixels())
        # 【已修改】绘制一个更醒目的标记：绿色外圈 + 白色十字
        img.draw_circle(biggest_blob.cx(), biggest_blob.cy(), 20, color=Config.HIGHLIGHT_COLOR, thickness=3)
        img.draw_cross(biggest_blob.cx(), biggest_blob.cy(), color=Config.WHITE_COLOR, size=20, thickness=1)
        # 添加坐标到列表
        all_coords_flat.extend([biggest_blob.cx(), biggest_blob.cy()])
    else:
        # 如果找不到色块，用0填充坐标
        all_coords_flat.extend([0, 0])

    # --- 3. 发送所有坐标 ---
    coord_string = ",".join(map(str, all_coords_flat))
    uart_comm.uart_send(coord_string)
    
    # --- 4. UI显示 ---
    # 更新并计算FPS
    state['fps_counter'] += 1
    if state['fps_counter'] >= 10:
        elapsed = time.ticks_diff(current_time, state['last_frame_time'])
        state['fps'] = 10000 / elapsed if elapsed > 0 else 0
        state['last_frame_time'] = current_time
        state['fps_counter'] = 0

    # 在左上角绘制信息
    img.draw_string_advanced(10, 10, 30, f"FPS: {state['fps']:.1f}", color=(255, 0, 255))
    img.draw_string_advanced(10, 45, 25, "长按进入调试", color=(0, 255, 255))
    
    # 显示灰度阈值 (用绿色)
    gray_text = f"灰度: ({rect_threshold[0]},{rect_threshold[1]})"
    img.draw_string_advanced(10, 80, 25, gray_text, color=(0, 255, 0))

    # 显示LAB颜色阈值 (也用绿色，表示都在工作中)
    lab_text_l = f"L:({lab_threshold[0]},{lab_threshold[1]})"
    lab_text_a = f"A:({lab_threshold[2]},{lab_threshold[3]})"
    lab_text_b = f"B:({lab_threshold[4]},{lab_threshold[5]})"
    img.draw_string_advanced(10, 110, 25, "LAB阈值:", color=(0, 255, 0))
    img.draw_string_advanced(20, 140, 25, lab_text_l, color=(0, 255, 0))
    img.draw_string_advanced(20, 170, 25, lab_text_a, color=(0, 255, 0))
    img.draw_string_advanced(20, 200, 25, lab_text_b, color=(0, 255, 0))
    
    
    rect_coords_str = f"Rect Ctr: {','.join(map(str, all_coords_flat[:4]))}"
    blob_coords_str = f"Blob Ctr: {','.join(map(str, all_coords_flat[4:]))}"

    img.draw_string_advanced(5, 440, 20, rect_coords_str, color=(0, 255, 255))
    img.draw_string_advanced(5, 460, 20, blob_coords_str, color=(0, 255, 255))
    
    Display.show_image(img, x=(Config.LCD_WIDTH - img.width()) // 2, y=0)

def handle_threshold_tuning(tp):
    """【已合并】支持灰度和LAB颜色阈值调试的界面。"""
    print("进入多模式阈值调试。")

    IMAGE_AREA_WIDTH = 480
    CONTROL_AREA_WIDTH = Config.LCD_WIDTH - IMAGE_AREA_WIDTH
    BUTTON_WIDTH = (CONTROL_AREA_WIDTH - 40) // 3
    BUTTON_HEIGHT, BUTTON_SPACING, BUTTON_Y = 50, 10, 10

    tuning_modes = {
        'grayscale': [('灰度最小', 0, 255), ('灰度最大', 0, 255)],
        'color': [
            ('L 最小', 0, 100), ('L 最大', 0, 100),
            ('A 最小', -128, 127), ('A 最大', -128, 127),
            ('B 最小', -128, 127), ('B 最大', -128, 127),
        ]
    }
    mode_keys = list(tuning_modes.keys())
    current_mode_idx = 0

    sliders = {}
    for mode, params in tuning_modes.items():
        sliders[mode] = []
        current_vals = state['thresholds'][mode][0]
        for i, (label, min_v, max_v) in enumerate(params):
            x_pos = IMAGE_AREA_WIDTH + 10
            y_pos = 100 + i * 60
            width = CONTROL_AREA_WIDTH - 20
            sliders[mode].append(SliderWidget(x_pos, y_pos, width, 50, label, min_v, max_v, current_vals[i]))

    def draw_ui_background(img, active_mode_name):
        img.draw_rectangle(0, 0, Config.LCD_WIDTH, Config.LCD_HEIGHT, color=Config.BG_COLOR, fill=True)
        img.draw_rectangle(IMAGE_AREA_WIDTH, 0, CONTROL_AREA_WIDTH, Config.LCD_HEIGHT, color=Config.PANEL_BG_COLOR, fill=True)
        button_start_x = IMAGE_AREA_WIDTH + 10
        
        img.draw_rectangle(button_start_x, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, color=Config.BUTTON_COLOR, fill=True)
        img.draw_string_advanced(button_start_x + BUTTON_WIDTH//2 - 20, BUTTON_Y + 15, 25, "返回", color=Config.TEXT_COLOR)
        
        button_x = button_start_x + BUTTON_WIDTH + BUTTON_SPACING
        img.draw_rectangle(button_x, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, color=Config.BUTTON_COLOR, fill=True)
        img.draw_string_advanced(button_x + BUTTON_WIDTH//2 - 20, BUTTON_Y + 15, 25, "切换", color=Config.TEXT_COLOR)

        button_x = button_start_x + 2 * (BUTTON_WIDTH + BUTTON_SPACING)
        img.draw_rectangle(button_x, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, color=Config.BUTTON_COLOR, fill=True)
        img.draw_string_advanced(button_x + BUTTON_WIDTH//2 - 20, BUTTON_Y + 15, 25, "保存", color=Config.TEXT_COLOR)

        mode_text = "灰度模式" if active_mode_name == 'grayscale' else "LAB颜色模式"
        img.draw_string_advanced(IMAGE_AREA_WIDTH + 10, 70, 25, mode_text, color=Config.TEXT_COLOR)

    while True:
        os.exitpoint()

        active_mode = mode_keys[current_mode_idx]
        active_sliders = sliders[active_mode]

        # img_raw = sensor.snapshot().copy(roi=Config.ROI)
        img_raw = sensor.snapshot().copy()
        current_vals = tuple(s.value for s in active_sliders)

        if active_mode == 'grayscale':
            img_processed = img_raw.to_grayscale()
            img_processed.binary([current_vals])
            img_processed = img_processed.to_rgb565()
        else: # color mode (LAB)
            img_processed = img_raw.copy()
            img_processed.binary([current_vals])

        canvas = image.Image(Config.LCD_WIDTH, Config.LCD_HEIGHT, image.RGB565)
        draw_ui_background(canvas, active_mode)

        preview_x = (IMAGE_AREA_WIDTH - img_processed.width()) // 2
        preview_y = (Config.LCD_HEIGHT - img_processed.height()) // 2
        canvas.draw_image(img_processed, preview_x, preview_y)

        for slider in active_sliders:
            slider.draw(canvas)

        points = tp.read()
        if len(points) > 0:
            x, y = points[0].x, points[0].y
            time.sleep_ms(120)

            if BUTTON_Y < y < BUTTON_Y + BUTTON_HEIGHT and x > IMAGE_AREA_WIDTH:
                button_start_x = IMAGE_AREA_WIDTH + 10
                if button_start_x <= x < button_start_x + BUTTON_WIDTH:
                    print("退出调试。")
                    return Config.MODE_NORMAL
                
                button_x = button_start_x + BUTTON_WIDTH + BUTTON_SPACING
                if button_x <= x < button_x + BUTTON_WIDTH:
                    current_mode_idx = (current_mode_idx + 1) % len(mode_keys)
                    continue
                
                button_x = button_start_x + 2 * (BUTTON_WIDTH + BUTTON_SPACING)
                if button_x <= x < button_x + BUTTON_WIDTH:
                    state['thresholds'][active_mode] = [current_vals]
                    print(f"准备保存所有配置: {state['thresholds']}")
                    
                    if config_manager.save_config(state['thresholds']):
                        msg, bg_color = "保存成功!", (200, 255, 200)
                    else:
                        msg, bg_color = "保存失败!", (255, 200, 200)
                    
                    save_msg_x = (Config.LCD_WIDTH - 240) // 2
                    save_msg_y = (Config.LCD_HEIGHT - 80) // 2
                    canvas.draw_rectangle(save_msg_x, save_msg_y, 240, 80, color=bg_color, fill=True)
                    canvas.draw_string_advanced(save_msg_x + 50, save_msg_y + 25, 30, msg, color=Config.TEXT_COLOR)
                    Display.show_image(canvas, 0, 0)
                    time.sleep_ms(1200)
                    continue

            for slider in active_sliders:
                adjustment = slider.check_touch(x, y)
                if adjustment != 0:
                    slider.update_value(adjustment)
                    break
        
        Display.show_image(canvas, 0, 0)


# --- 7. 主程序 ---
def main():
    try:
        global sensor, state
        sensor = Sensor(width=Config.SENSOR_WIDTH, height=Config.SENSOR_HEIGHT)
        sensor.reset()
        sensor.set_framesize(width=Config.SENSOR_WIDTH, height=Config.SENSOR_HEIGHT)
        sensor.set_pixformat(Sensor.RGB565)
        Display.init(Display.ST7701, to_ide=True, width=Config.LCD_WIDTH, height=Config.LCD_HEIGHT)
        MediaManager.init()
        sensor.run()
        tp = TOUCH(0)

        loaded_thresholds = config_manager.load_config()
        if loaded_thresholds:
            state['thresholds'].update(loaded_thresholds)
            print(f"已应用SD卡中的配置: {state['thresholds']}")
        else:
            print(f"使用默认配置: {state['thresholds']}")

        print("初始化完成，长按屏幕进入阈值调试模式。")
        state['last_frame_time'] = time.ticks_ms()

        uart_comm.uart_send(uart_comm.uart_package)
        while True:
            os.exitpoint()
            uart_comm.uart_package=uart_comm.uart_rece()
            if state['mode'] == Config.MODE_NORMAL:
                points = tp.read()
                if len(points) > 0:
                    state['touch_counter'] += 1
                    if state['touch_counter'] > Config.TOUCH_HOLD_FRAMES:
                        state['mode'] = Config.MODE_TUNING
                        state['touch_counter'] = 0
                else:
                    state['touch_counter'] = max(0, state['touch_counter'] - 2)

            if state['mode'] == Config.MODE_NORMAL:
                handle_normal_mode()
            elif state['mode'] == Config.MODE_TUNING:
                state['mode'] = handle_threshold_tuning(tp)

    except KeyboardInterrupt:
        print("用户终止程序。")
    except Exception as e:
        sys.print_exception(e)
        print(f"发生致命错误: {e}")
    finally:
        if 'sensor' in globals() and sensor:
            sensor.stop()
        Display.deinit()
        MediaManager.deinit()
        os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
        print("系统清理并停止。")

if __name__ == "__main__":
    main()
