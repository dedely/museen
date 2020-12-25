<?php
require_once "./include/functions.inc.php";
session_start();
check_sign_in_redirect();
$title = "Préférences · museen";
require_once "./include/head.inc.php";
?>

<body class="text-center">

    <h1 class="h3 mb-3 font-weight-normal">Choisissez vos mouvements artistiques préférés</h1>
    <section>
        <h2 class ="h4 mb-3 font-weight-normal">Aidez nous à personnaliser votre expérience</h2>
        <form class="form-signin mb-3" method="GET" action="preferencesresults.php">
            <?php
            for ($i = 1; $i <= 3; $i++) {
                show_movements_form($i);
            }
            ?>
            <div class="form-row mb-3">
                <button class="btn btn-md btn-primary btn-block mx-auto" type="submit">Valider</button>
            </div>
        </form>
    </section>

    <?php require_once "./include/footer.inc.php" ?>
