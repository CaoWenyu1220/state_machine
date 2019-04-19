横着写（在事件中判断状态）C代码片段：

//e0事件发生时，执行的函数
void e0_event_function(int * nxt_state)
{   
    int cur_state;   
    cur_state = *nxt_state;   
    switch(cur_state)
    {       
        case s0: //观察表1，在e0事件发生时，s1处为空   
        case s2: //执行a0动作;           
        *nxt_state = s0;
    }
}

//e1事件发生时，执行的函数
void e1_event_function(int * nxt_state)
{   
    int cur_state;   
    cur_state = *nxt_state;   
    switch(cur_state)
    {       
        case s0: //观察表1，在e1事件发生时，s1和s2处为空           
            //执行a1动作;           
            *nxt_state = s1;
    }
}

//e2事件发生时，执行的函数
void e2_event_function(int * nxt_state)
{   
    int cur_state;   
    cur_state = *nxt_state;   
    switch(cur_state)
    {       
        case s0: //观察表1，在e2事件发生时，s2处为空       
        case s1:           
            //执行a2动作;           
            *nxt_state = s2; 
    }
}
