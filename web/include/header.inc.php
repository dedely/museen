<?php
require_once "include/functions.inc.php";
session_start();
check_remember_cookie();
?>
<!DOCTYPE html>
<html lang="fr">

<head>
    <meta charset="utf-8" />
    <meta name="author" content="Adel Abbas, Aelien Moubeche" />
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
    <meta name="date" content=<?php echo "\"" . date('D, d M Y') . "\"" ?> />
    <meta name="keywords" content="Museum, art, visite guidée, museen, <?php echo $title ?>" />
    <meta name="description" content="" />
    <title><?php echo $title ?></title>

    <!-- Bootstrap core CSS -->
    <link rel="stylesheet" href="css/bootstrap.min.css"/>
</head>

<body>
    <div class="d-flex flex-column flex-md-row align-items-center p-3 px-md-4 mb-3 bg-white border-bottom shadow-sm">
        <a class="navbar-brand my-0 mr-md-auto font-weight-normal" href="#">
            <img src="/images/logologo.png" width="30" height="30" class="d-inline-block align-top" alt="" loading="lazy" />
            Museen
        </a>
        <nav class="my-2 my-md-0 mr-md-3">
            <a class="p-2 text-dark" href="#">Infos</a>
            <a class="p-2 text-dark" href="#">Nos oeuvres</a>
            <a class="p-2 text-dark" href="#">Réservation</a>
            <?php
            if (is_signed_in()) {
                echo "<a class=\"p-2 text-dark\" href=\"account.php\">Mon compte</a>\n";
            }
            ?>
        </nav>
        <?php
        if (is_signed_in()) {
            show_disconnect_button();
        }else{
            show_signin_button();
        }
        ?>
    </div>