<?php
session_start();
unset($_COOKIE["remember"]);
unset($_SESSION["user"]);
session_destroy();
header("location:index.php");
