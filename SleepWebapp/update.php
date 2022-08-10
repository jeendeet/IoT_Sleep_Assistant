<?php
    $hr     = 0;
    $mov    = 0;
    $count  = 0;
    date_default_timezone_set('Asia/Ho_Chi_Minh');
    function write_status($hrx, $movx, $countx){
        $myfile = fopen("health.txt", "w");
        fwrite($myfile,$hrx);
        fwrite($myfile,$movx);
        fwrite($myfile,$countx);
        
        fclose($myfile);
    }
    function write_history($hrx, $movx, $countx){
        $myfile = fopen("history.csv", "a");
        $time = date("d-m-Y H:i:s").",";
        fwrite($myfile,$time);
        fwrite($myfile,$hrx);
        fwrite($myfile,$movx);
        fwrite($myfile,$countx);
        fwrite($myfile,"\n");
        fclose($myfile);
    }
    
    if (isset($_GET["update"])){
        $hr     = $_GET["hr"].",";
        $mov    = $_GET["mov"].",";
        $count  = $_GET["count"].",";
        write_status($hr, $mov, $count);
        write_history($hr, $mov, $count);
    }
    echo date("d-m-Y H:i:s");
?>