<?php
require_once "./include/functions.inc.php";
session_start();
check_sign_in_redirect();
$title = "Mon compte · museen";
require_once "./include/header.inc.php";
?>

<body class=text-center>
    <h1 class="mx-auto">Votre compte, tout simplement.</h1>
    <?php
    $visitor_names = get_visitor_name($_SESSION["user"]);
    echo "<p>Bienvenue " . $visitor_names["visitor_first_name"] . " " . $visitor_names["visitor_last_name"] . "</p>\n";
    ?>
    <p>Plus de contenu sera bientôt disponible sur cette page! Merci de votre confiance.</p>
    <figure class="figure">
        <img src="./images/welcome.png" alt="Bienvenue!" id="coming_soon_img" class="figure-img img-fluid rounded" width="40%" height="40%"/>
    </figure>

    <?php require_once "./include/footer.inc.php"; ?>
