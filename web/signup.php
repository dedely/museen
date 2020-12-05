<?php
require_once "./include/functions.inc.php";
session_start();
signup();
$title = "Inscription · museen";
$stylesheet = "./css/signin.css";
require_once "./include/head.inc.php";
?>

<body class="text-center">
    <form class="mx-auto" method="POST" action="signup.php ">
        
        <a class="mb-4" href="index.php">
            <img class="mb-4" src="images/logologo.png" alt="Museen Logo" width="72" height="72" />
        </a>
        <h1 class="h3 mb-3 font-weight-normal">Inscription</h1>
        <div class="form-row">
            <div class="form-group md-4">
                <label for="inputLogin">Identifiant</label>
                <input type="text" id="inputLogin" name="id" class="form-control" required="required" autofocus="autofocus" />
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label for="inputFirstName">Prénom</label>
                <input type="text" id="inputFirstName" name="firstName" class="form-control" required="required" />
            </div>
            <div class="form-group">
                <label for="inputLastName">Nom</label>
                <input type="text" id="inputLastName" name="lastName" class="form-control" required="required" />
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label for="inputBirthdate">Date de naissance</label>
                <input type="date" id="inputBirthdate" name="birthdate" class="form-control" required="required" />
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label for="inputPassword">Mot de passe</label>
                <input type="password" id="inputPassword" name="password" class="form-control" required="required" />
            </div>
        </div>
        <div class="form-row">
            <button class="btn btn-lg btn-primary btn-block" type="submit">Inscription</button>
        </div>
    </form>
</body>

</html>