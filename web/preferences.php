<?php
require_once "include/functions.inc.php";
session_start();
check_sign_in_redirect();
$title = "Préférences · museen";
require_once "include/head.inc.php";
?>

<body>

    <h1>Choisissez vos mouvements artistiques préférés</h1>
    <section>
        <h2>Aidez nous à personnaliser votre expérience</h2>
        <form method="GET" action="preferencesresults.php">
            <?php
            for ($i = 1; $i <= 3; $i++) {
                show_movements_form($i);
            }
            ?>
            <div class="form-row">
                <button class="btn btn-lg btn-primary btn-block" type="submit">Valider</button>
            </div>
        </form>
    </section>

</body>

</html>