<?php
session_start();
include "include/functions.inc.php";
login_user();
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
    <link rel="stylesheet" href="./signin.css" />
</head>

<body class="text-center">
    <form class="form-signin" method="POST" action="login.php">
        <h1 class="h3 mb-3 font-weight-normal">Connectez-vous</h1>
        <label for="inputLogin" class="sr-only">Login</label>
        <input type="text" id="inputLogin" name="id" class="form-control" placeholder="Identifiant" required="required" autofocus="autofocus" />
        <label for="inputPassword" class="sr-only">Mot de passe</label>
        <input type="password" id="inputPassword" name="password" class="form-control" placeholder="Mot de passe" required="required" />
        <div class="checkbox mb-3">
            <label>
                <input type="checkbox" value="remember-me"> Rester connecté</input>
            </label>
        </div>
        <button class="btn btn-lg btn-primary btn-block" type="submit">Connexion</button>
    </form>
</body>

</html>