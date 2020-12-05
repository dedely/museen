<?php
require_once "./include/functions.inc.php";
session_start();
signin();
$title = "Connexion · museen";
$stylesheet = "./css/signin.css";
require_once "./include/head.inc.php";
?>

<body class="text-center">
    <form class="form-signin" method="POST" action="signin.php">
        <a href="index.php">
            <img class="mb-4" src="images/logologo.png" alt="Museen Logo" width="72" height="72" />
        </a>
        <h1 class="h3 mb-3 font-weight-normal">Connectez-vous</h1>
        <label for="inputLogin" class="sr-only">Login</label>
        <input type="text" id="inputLogin" name="id" class="form-control" placeholder="Identifiant" required="required" autofocus="autofocus" />
        <label for="inputPassword" class="sr-only">Mot de passe</label>
        <input type="password" id="inputPassword" name="password" class="form-control" placeholder="Mot de passe" required="required" />
        <div class="checkbox mb-3">
            <label for="remember">Rester connecté</label>
            <input type="checkbox" id="remember" name="remember" value="remember-me" />
        </div>
        <button class="btn btn-lg btn-primary btn-block" type="submit">Connexion</button>
        <p class="mt-5 mb-3">Pas encore inscrit? <a href="signup.php">S'inscrire.</a></p>
    </form>
</body>

</html>