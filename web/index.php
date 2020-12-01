<?php
  $title = "Museen, le musée connecté.";
  require_once "include/header.inc.php";
  login_user();
  connect_db();
  $session = isset($_SESSION["user"]);
  var_dump($session);
?>


</body>

</html>