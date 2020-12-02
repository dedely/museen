<?php
session_start();
$title = "Connexion · museen";
$stylesheet = "signin.css";
require_once "include/head.inc.php";
signin();
?>

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
    <div class="class=mx-auto">
        <p>Pas inscrit? <a href="signup.php">S'inscrire.</a></p>
    </div>

</body>

</html>