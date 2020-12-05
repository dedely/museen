<?php
require_once "./include/functions.inc.php";
session_start();
check_remember_cookie();
$title = "Museen, le musée connecté.";
require_once "./include/header.inc.php";
?>

<div class="jumbotron jumbotron-fluid">
  <div class="container">
    <h1 class="display-4">Bienvenue chez museen</h1>
    <p class="lead">Le musée connecté.</p>
  </div>
</div>

<section class="text-center">
  <h2>Une collection riche pour des goûts variés</h2>
  <div>
    <?php
    display_popular_artworks();
    ?>
  </div>

</section>
<?php require_once "./include/footer.inc.php" ?>


