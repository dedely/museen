<?php
require_once "include/functions.inc.php";
session_start();
check_sign_in_redirect();
handle_preferences();
$title = "Préférences · museen";
require_once "include/head.inc.php";
?>

<body>
    <h1>Vous y êtes presque!</h1>
    <section>
        <h2>Choisissez vos artistes préférés</h2>
        <form method="GET" action="preferencesresults.php">
            <?php

            for ($i = 1; $i <= 3; $i++) {
                show_artists_form($i, $_SESSION["user"]);
            }
            ?>
            <div class="form-row">
                <button class="btn btn-lg btn-primary btn-block" type="submit">Valider</button>
            </div>
        </form>
    </section>
</body>

<?php require_once "include/footer.inc.php" ?>