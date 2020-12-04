<?php
$title = "Préférences · museen";
require_once "include/head.inc.php";
?>

<body>

    <h1>Choisissez vos mouvements artistiques préférés</h1>
    <section>
        <h2>Aidez nous à personnaliser votre expérience</h2>
        <form>
            <?php
            $mouvements = get_artistic_movements();
            for ($i = 0; $i < count($mouvements); $i++){
                echo "\t\t\t<div class=\"form-row\">\n";
                    echo "\t\t\t\t<div class=\"form-group\">\n";
                        echo "\t\t\t\t\t<label for=\"".$mouvements[$i] . "Range"."\"> ".$mouvements[$i]."</label>\n";
                        echo "\t\t\t\t\t<input type=\"range\" id=\"".$mouvements[$i] . "Range\" name=\"$mouvements[$i]\" class=\"custom-range\" min=\"1\" max=\"5\" />\n";
                    echo "\t\t\t\t</div>\n";
                echo"\t\t\t</div>\n";
            }

            ?>
            <div class="form-row">
                <button class="btn btn-lg btn-primary btn-block" type="submit">Valider</button>
            </div>
        </form>
    </section>

</body>