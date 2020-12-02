<?php
require_once "include/functions.inc.php";
session_start();
signup();
$title = "Inscription · museen";
require_once "include/head.inc.php";
?>

<body>
    <form method="POST" action="signup.php">
        <div class="form-row">
            <div class="form-group">
                <label for="inputLogin">Login</label>
                <input type="text" id="inputLogin" name="id" class="form-control" required="required" autofocus="autofocus" />
            </div>
        </div>
        <div class="form-row align-items-center">
            <div class="form-group">
                <label for="inputFirstName">Prénom</label>
                <input type="text" id="inputFirstName" name="firstName" class="form-control" required="required"/>
            </div>
            <div class="form-group">
                <label for="inputLastName">Nom</label>
                <input type="text" id="inputLastName" name="lastName" class="form-control" required="required"/>
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label for="inputBirthdate">Date de naissance</label>
                <input type="date" id="inputBirthdate" name="birthdate" class="form-control" required="required"/>
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label for="inputPassword">Mot de passe</label>
                <input type="password" id="inputPassword" name="password" class="form-control" required="required" />
            </div>
        </div>
        <div class="form-row">
            <button class="btn btn-lg btn-primary btn-block">Inscription</button>
        </div>
    </form>
</body>