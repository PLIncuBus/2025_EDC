#include "Path_Planning.h"

char end_quene = 1;
void test()
{
		while(1)
		{
			Menu_Process();
		}
}

static void Path_Planning_Solve_Li5th_Node(Path_Plan_Li5_t *node);
static float Path_Planning_Control(Path_Plan_Quene_t *li5_path,Path_Plan_Out_t choose_out_type);

#if (Chassis_mode == Differential_Wheel)
//     float time;                     //节点时间
//		 float dis;                      //节点位移
//     float vel;                      //节点速度
//     float acc;                      //节点加速度
//     void * task;                    //节点任务
    static Path_Plan_Li5_t Diff_node_x_speed[] = {	{.task=&test},
//																									{0.0f, 0.0f, 0.0f, 0.0f, NULL},
//																									{2000.0f,45000.0f,10.0f,0.0f,NULL},
//																									{3000.0f,45000.0f, 0.0f, 0.0f, NULL},
//																									{5000.0f,90000.0f, 10.0f, 0.0f, NULL},
//																									{6000.0f,90000.0f, 0.0f, 0.0f, NULL},
//																									{8000.0f,135000.0f, 10.0f, 0.0f, NULL},
//																									{9000.0f,135000.0f, 0.0f, 0.0f, NULL},
//																									{11000.0f,180000.0f, 10.0f, 0.0f, NULL},
//																									
																										
                                            {.task = &end_quene}};
    static Path_Plan_Li5_t Diff_node_w_speed[] = {{0.0f, 0.0f, 0.0f, 0.0f, NULL},
																									{2000.0f,0.0f,0.0f,0.0f,NULL},
																									{3000.0f,-90.0f,0.0f,0.0f,NULL},
																									{5000.0f, -90.0f, 0.0f, 0.0f, NULL},
																									{6000.0f, -180.0f, 0.0f, 0.0f, NULL},
																									{8000.0f, -180.0f, 0.0f, 0.0f, NULL},
																									{9000.0f, -270.0f, 0.0f, 0.0f, NULL},
																									{11000.0f, -270.0f, 0.0f, 0.0f, NULL},
																									
                                            {.task = &end_quene}};

    static Path_Plan_Quene_t Diff_x_speed = {.quene = Diff_node_x_speed,
                                   .timer.count_end_time = 0xFFFFFFFF};

    static Path_Plan_Quene_t Diff_w_dis = {.quene = Diff_node_w_speed,
                                 .timer.count_end_time = 0xFFFFFFFF};


#elif (Chassis_mode == Mecanum_Wheel)

    static Path_Plan_Li5_t Mec_node_x_speed[] = {{.task = NULL},
                                            {.task = &end_quene}};
    static Path_Plan_Li5_t Mec_node_y_speed[] = {{.task = NULL},
                                            {.task = &end_quene}};                                           
    static Path_Plan_Li5_t Mec_node_w_speed[] = {{.task = NULL},
                                            {.task = &end_quene}};
    static Path_Plan_Quene_t Mec_x_speed = {.quene = Mec_node_x_speed,
                                   .timer.count_end_time = 0xFFFFFFFF};                                  
    static Path_Plan_Quene_t Mec_y_speed = {.quene = Mec_node_y_speed,
                                   .timer.count_end_time = 0xFFFFFFFF};
    static Path_Plan_Quene_t Mec_w_dis = {.quene = Mec_node_w_speed,
                                 .timer.count_end_time = 0xFFFFFFFF};


#elif (Chassis_mode == None )

    static Path_Plan_Li5_t Task_node[] = {{.task = NULL},
                                        {.task = &end_quene}};

#endif


/**
 * @brief 路径规划初始化
 * @note 主函数初始化的时候调用
 */
void Path_Planning_Init(void)
{
    Path_Planning_Solve_Li5th_Node(Diff_x_speed.quene);
    Path_Planning_Solve_Li5th_Node(Diff_w_dis.quene);

    SystemClock_start_count_time(&Diff_x_speed.timer);
    SystemClock_start_count_time(&Diff_w_dis.timer);
}

/**
 * @brief 路径规划发布数据
 * @note 主函数while循环中调用
 * @param _Path_Planning_Publish 
 */
void Path_Planning_Publish(Differential_Wheel_Info_t * _Path_Planning_Publish)
{
    _Path_Planning_Publish->vx_set    = Path_Planning_Control(&Diff_x_speed,out_type_speed);
    _Path_Planning_Publish->angle_set = Path_Planning_Control(&Diff_w_dis,out_type_displacement);      
}

/**
 * @brief 路径规划五阶线性插值节点解算
 * @param node 节点列表
 * @return void
 */
static void Path_Planning_Solve_Li5th_Node(Path_Plan_Li5_t *node)
{
    input_li5_t input;
    char i = 0;

    while(1)
    {
        if(node[i+1].task == NULL)
        {
            input.t0 = node[i].time;
            input.s0 = node[i].dis;
            input.v0 = node[i].vel;
            input.a0 = node[i].acc;

            input.t1 = node[i+1].time;
            input.s1 = node[i+1].dis;
            input.v1 = node[i+1].vel;
            input.a1 = node[i+1].acc;

            node[i+1].solve = li5th_solve(input);
        }
        else if(*(char *)node[i+1].task == end_quene) break;

        i++;
    }
}

/**
 * @brief 路径规划控制
 * @param li5_path            路径队列
 * @param choose_out_type     选择输出数据类型
 * @return 无
 */
static float Path_Planning_Control(Path_Plan_Quene_t *li5_path,Path_Plan_Out_t choose_out_type)
{
    SystemClock_Get_State(&li5_path->timer);
    //根据时间推算节点
    if(((li5_path->quene)[li5_path->p_node].time) <= (&li5_path->timer)->count_now_time)
    {
        //该节点有任务
        if((li5_path->quene)[li5_path->p_node+1].task != NULL) 
        {
            //路径任务完成
            if(*(char *)(li5_path->quene)[li5_path->p_node+1].task == end_quene) 
                return 0;
                          //运行节点任务
            ((void (*)(void))((li5_path->quene)[li5_path->p_node+1].task))();
            SystemClock_start_count_time(&li5_path->timer);
            li5_path->p_node ++;
        }
        li5_path->p_node ++;
    }
        if(choose_out_type == out_type_speed)
        return get_speed_from_li5(&((li5_path->quene)[li5_path->p_node].solve), (&li5_path->timer)->count_now_time);
    else if(choose_out_type == out_type_displacement)
        return get_displacement_from_li5(&((li5_path->quene)[li5_path->p_node].solve), (&li5_path->timer)->count_now_time);
    else 
        return get_acceleration_from_li5(&((li5_path->quene)[li5_path->p_node].solve), (&li5_path->timer)->count_now_time);

}










