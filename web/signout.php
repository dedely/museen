<?php
session_start();
unset($_COOKIE["remember"]);
session_destroy();
header("location:index.php");
?>