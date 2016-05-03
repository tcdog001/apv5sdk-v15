BEGIN{
    flag=0;
    FS="<|>|</"
}
{
    if(flag==1)
    {
        if($0 !~ option1)
        {
           if($0 ~ option2 )
           {
              print $3
           } 
        }
    }
    if($0 ~ option1)
    {
        if(flag==0)
        {
            flag=1;
        }
        else
        {
            flag=0;
        }
    }
}
