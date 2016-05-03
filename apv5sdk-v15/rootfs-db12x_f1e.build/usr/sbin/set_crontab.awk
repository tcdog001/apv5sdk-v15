BEGIN{
  TARGET=""
  INTERVAL=0
  OPTION=""
}
{
    if($0 !~ TARGET )
    {
        print $0;
    }
}
END{
    if(INTERVAL !=0)
    {
       SUB_INTERVAL = substr(INTERVAL,2,length(INTERVAL)-1)
       if(SUB_INTERVAL != 0)
       {
           if(substr(INTERVAL,1,1) == "h")
           {
               print "* */"SUB_INTERVAL" * * * "TARGET" "OPTION
           } 
           else
           {
               print "*/"SUB_INTERVAL" * * * * "TARGET" "OPTION
           }
       }
    }
}


