<?php
session_start();
unset($_COOKIE["remember"]);
setcookie("remember", null, -1, '/'); 
unset($_SESSION["user"]);
session_destroy();
header("location:index.php");
