　　竖着写（在状态中判断事件）C代码片段：

cur_state = nxt_state;   
switch(cur_state) //在当前状态中判断事件
{            
    case s0: //在s0状态   
        if(e0_event) //如果发生e0事件，那么就执行a0动作，并保持状态不变；
        {   
　　　　    //执行a0动作;               
　　　　    //nxt_state = s0;  //因为状态号是自身，所以可以删除此句，以提高运行速度。
        } 
        else if(e1_event) //如果发生e1事件，那么就执行a1动作，并将状态转移到s1态；
        {   
            //执行a1动作;
            nxt_state = s1;
        }           
        else if(e2_event) //如果发生e2事件，那么就执行a2动作，并将状态转移到s2态；
        {  
            //执行a2动作;
            nxt_state = s2;
        }
        else
        {
            break;    
        }   

    case s1: //在s1状态
        if(e2_event) //如果发生e2事件，那么就执行a2动作，并将状态转移到s2态； 
        {                
            //执行a2动作;
      　　　nxt_state = s2;
        }           
        else
        {
　　　　　　break;
        }

    case s2: //在s2状态
        if(e0_event)  //如果发生e0事件，那么就执行a0动作，并将状态转移到s0态；
        {
            //执行a0动作;               
            nxt_state = s0;
        }
}
