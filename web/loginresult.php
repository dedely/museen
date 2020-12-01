<?php
session_start();
include "include/functions.inc.php";
?>

<!DOCTYPE html>
<html lang="fr">

<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
    <meta name="description" content="" />
    <meta name="author" content="Adel Abbas, Aelien Moubeche" />
    <title>Connexion · museen</title>

    <!-- Bootstrap core CSS -->
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous" />
    <!-- Custom styles for this template -->
    <!--<link rel="stylesheet" href="./signin.css" />-->
</head>

<body class="text-center">
    <?php
    login_user();
    if (isset($_SESSION["user"])) {
        var_dump($_SESSION["user"]);
    } else {
        echo "no\n";
    }
    ?>
</body>

</html>