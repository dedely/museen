<?php
require_once "include/postgresql.conf.inc.php";
define("HASHING_ALGO", "sha256");
define("DEBUG", false);
define("AUTHKEY_LENGTH", 15);

/**
 * This function handles user signin
 *
 * @param string $referrer the page the user was on before accessing the signin form
 * @return void
 */
function signin(string $referrer = "index.php"): void
{
    if (isset($_POST["id"], $_POST["password"])) {
        $id = $_POST["id"];
        $password = $_POST["password"];
        $password_hash = hash(HASHING_ALGO, $password);
        if (is_visitor_id($id) === true) {
            $query = "SELECT visitor.visitor_id, visitor.visitor_password_hash
                         FROM visitor 
                         WHERE visitor.visitor_id = '" . $id . "'
                         AND visitor.visitor_password_hash = '" . $password_hash . "';";
            $result = make_query($query);
            $nbrows = pg_numrows($result);
            if ($nbrows == 1) {
                set_session_user($id);
                remember_cookie();
                header("location:" . $referrer);
            }
        } else {
            $_SESSION["signinError"] = true;
        }
    }
}

/**
 * This function handles user signup
 *
 * @return void
 */
function signup(): void
{
    $errors = array();
    if (isset($_POST["id"], $_POST["firstName"], $_POST["lastName"], $_POST["birthdate"], $_POST["password"])) {
        $id = $_POST["id"];
        $firstName = $_POST["firstName"];
        $lastName = $_POST["lastName"];
        $birthDate = $_POST["birthdate"];
        $password = $_POST["password"];

        $password_hash = hash("sha256", $password);
        $authkey = generate_random_string(AUTHKEY_LENGTH);
        $authkey_hash = hash("sha256", $authkey);


        if (!is_smaller_str($firstName, 20)) {
            $errors["firstNameError"] = true;
        }
        if (!is_smaller_str($lastName, 20)) {
            $errors["lastNameError"] = true;
        }
        if (!is_visitor_id($id)) {
            $errors["idError"] = true;
        }
        if (empty($errors)) {
            $query = "INSERT INTO public.visitor (visitor_id, visitor_first_name, visitor_last_name, visitor_birthdate, visitor_password_hash,
            visitor_authkey_hash) VALUES ('" . $id . "', '" . $firstName . "', '" . $lastName . "', '" . $birthDate . "', '" . $password_hash . "', '" . $authkey_hash . "');";
            $result = make_query($query);
            $status = pg_result_status($result);
            if ($status == PGSQL_COMMAND_OK) {
                set_session_user($id);
                //Save the authkey to be able to display it later to the user.
                set_session_authkey($authkey);
                header("location:preferences.php");
            }
        }
    }
    set_session_errors($errors);
}

/**
 * Queries the database to get an array of the existing artistic movements
 *
 * @return array
 */
function get_artistic_movements(): array
{
    $query = "SELECT artistic_movement_id, artistic_movement_name FROM artistic_movement;";
    $movements = array();
    if (($result = make_query($query)) != FALSE) {
        while ($movement = pg_fetch_row($result, NULL, PGSQL_ASSOC)) {
            $movements[] = $movement;
        }
    }
    return $movements;
}

function get_visitor_name(string $id): array
{
    $query = "SELECT visitor_first_name, visitor_last_name FROM visitor WHERE visitor_id = '" . $id . "';";
    $visitor_names = array();
    if (($result = make_query($query)) != FALSE) {
        $visitor_names = pg_fetch_row($result, NULL, PGSQL_ASSOC);
    }
    return $visitor_names;
}

/**
 * Queries the database to get an array of popular artworks
 *
 * @return array
 * @author Aëlien
 */
function get_popular_artworks(): array
{
    //query will get all sorting columns. will need filter afterwards
    $query = "SELECT artwork_id, artwork_title, artwork_popularity, artwork_movement_id, artistic_movement_name";
    $query .= " FROM artwork JOIN artistic_movement ON artwork_movement_id = artistic_movement_id";
    $query .= " WHERE (artwork_movement_id, artwork_popularity) IN (SELECT artwork_movement_id, MAX(artwork_popularity) FROM artwork";
    $query .= " GROUP BY artwork_movement_id)";
    $query .= " ORDER BY artwork_movement_id;";

    $artworks = array();
    if (($result = make_query($query)) != FALSE) {
        while ($artwork = pg_fetch_row($result, NULL, PGSQL_ASSOC)) {
            $artworks[] = $artwork;
        }
    }
    return $artworks;
}

/**
 * Queries the database to get an array of the possible preferred artits
 *
 * @param string $id Precondition $id must be valid
 * @return array | false if no $id provided
 */
function get_possible_preferred_artists(string $id): array
{
    $artists = array();
    if (isset($id)) {
        $query = "SELECT AR.artist_id, AR.artist_first_name, AR.artist_last_name, AVG(ATWK.artwork_popularity) AS ARTIST_POP, AMP.amp_score ";
        $query .= " FROM artist AS AR JOIN artwork AS ATWK ON AR.artist_id = ATWK.artwork_artist ";
        $query .= " JOIN artistic_movement_preference AS AMP ON ATWK.artwork_movement_id = AMP.amp_artistic_movement_id JOIN visitor AS V ON V.visitor_id = AMP.amp_visitor_id";
        $query .= " WHERE AMP.amp_visitor_id = '" . $id . "'";
        $query .= " GROUP BY AR.artist_id, AMP.amp_score";
        $query .= " ORDER BY AMP.amp_score DESC, ARTIST_POP DESC;";
        if (($result = make_query($query)) != FALSE) {
            while ($artist = pg_fetch_row($result, NULL, PGSQL_ASSOC)) {
                $artists[] = $artist;
            }
        }
    }
    return $artists;
}

/**
 * Stores artistic movement preferences
 * Precondition : preferences should not have been set before for the provided visitor
 *
 * @param string $visitor_id
 * @param array $preferences array of amp_artistic_movement_id
 * @return void
 */
function store_movement_preferences(string $visitor_id, array $preferences): bool
{
    $score = 5;
    $query = "INSERT INTO artistic_movement_preference(amp_visitor_id, amp_artistic_movement_id, amp_score)";
    $query .= " VALUES\n";
    for ($i = 0; $i < count($preferences) - 1; $i++) {
        $amp_artistic_movement_id = $preferences[$i];
        $query .= "('" . $visitor_id . "', " . $amp_artistic_movement_id . ", " . $score . "),\n";
        if ($score > 1) {
            $score--;
        }
    }
    $query .= "('" . $visitor_id . "', " . $preferences[$i] . ", " . $score . ");";
    $result = make_query($query);
    return pg_result_status($result) === PGSQL_COMMAND_OK;
}

/**
 * Process the movement preferences form
 *
 * @return void
 */
function handle_preferences(): void
{
    $validInput = false;
    if (isset($_GET["movementSelect1"], $_GET["movementSelect2"], $_GET["movementSelect3"])) {
        $a = $_GET["movementSelect1"];
        $b = $_GET["movementSelect2"];
        $c = $_GET["movementSelect3"];
        $validInput = ($a !== $b) &&  ($a !== $c) && ($b !== $c);
    }
    if ($validInput && !($already_set = are_set_movement_preferences($_SESSION["user"]))) {
        $preferences = array($a, $b, $c);
        $res = store_movement_preferences($_SESSION["user"], $preferences);
    } else if ($already_set) {
        header("location:index.php");
    } else {
        header("location:preferences.php");
    }
}

/*----------------Display----------------*/

function display_popular_artworks(): void
{
    $artworks = get_popular_artworks();
    echo "<table class=\"table table-striped\">\n";
    echo "<thead>\n";
    echo "<tr>\n";
    echo "\t<th scope=\"col\">#</th>\n";
    echo "\t<th scope=\"col\">Mouvement artistique</th>\n";
    echo "\t<th scope=\"col\">Titre de l'oeuvre</th>\n";
    echo "</tr>\n";
    echo "</thead>\n";
    echo "<tbody>\n";
    foreach ($artworks as $key => $artwork) {
        echo "<tr>\n";
        echo "\t<th scope=\"row\">" . $artwork["artwork_movement_id"] . "</th>\n";
        echo "\t<td>" . $artwork["artistic_movement_name"] . "</td>\n";
        echo "\t<td>" . $artwork["artwork_title"] . "</td>\n";
        echo "</tr>";
    }
    echo "</tbody>\n";
    echo "</table>\n";
}

function show_movements_form(int $movementSelectId = 1): void
{
    $movements = get_artistic_movements();
    if (!empty($movements)) {
        $cpt = count($movements);
        echo "<select name=\"movementSelect" . $movementSelectId . "\" id=\"movementSelect" . $movementSelectId . "\" >\n";
        echo "\t\t\t<option value=\"none\" selected=\"selected\" disabled=\"disabled\" hidden=\"hidden\">Préférence n°" . $movementSelectId . "</option>\n";
        for ($i = 0; $i < $cpt; $i++) {
            echo "\t\t\t<option value=\"" . $movements[$i]["artistic_movement_id"] . "\">" . $movements[$i]["artistic_movement_name"] . "</option>\n";
        }
        echo "\t\t</select>\n";
    }
}

function show_artists_form(int $artistSelectId = 1, string $visitor_id): void
{
    $artists = get_possible_preferred_artists($visitor_id);
    echo "<select name=\"artistSelect" . $artistSelectId . "\" id=\"artistSelect" . $artistSelectId . "\" >\n";
    echo "\t\t\t<option value=\"none\" selected=\"selected\" disabled=\"disabled\" hidden=\"hidden\">Préférence n°" . $artistSelectId . "</option>\n";
    foreach ($artists as $artist) {
        echo "\t\t\t<option value=\"" . $artist["artist_id"] . "\">" . $artist["artist_first_name"] . " " . $artist["artist_last_name"] . "</option>\n";
    }

    echo "\t\t</select>\n";
}


function show_signin_button(): void
{
    echo "<a class=\"btn btn-outline-primary\" href=\"./signin.php\">Connexion</a>\n";
}

function show_disconnect_button(): void
{
    echo "<a class=\"btn btn-outline-secondary\" href=\"./signout.php\">Déconnexion</a>\n";
}

/**
 * depreciated
 *
 * @return void
 */
function show_account_button(): void
{
    echo "<div class=\"input-group-prepend\">\n";
    echo "\t<button class=\"btn btn-outline-secondary dropdown-toggle\" type=\"button\" data-toggle=\"dropdown\" aria-haspopup=\"true\" aria-expanded=\"false\">";
    echo $_SESSION["user"] . "</button>\n";
    echo "\t<div class=\"dropdown-menu\">\n";
    echo "\t\t<a class=\"dropdown-item\" href=\"account.php\">Mon compte</a>\n";
    echo "\t\t<a class=\"dropdown-item\" href=\"signout.php\">Déconnexion</a>\n";
    echo "\t</div>\n";
    echo "</div>\n";
}

/***********COOKIES*************/

/**
 * Remember the user
 *
 * @return void
 */
function remember_cookie(): void
{
    if (isset($_POST["remember"], $_SESSION["user"])) {
        if ($_POST["remember"] === "remember-me") {
            $time = time() + 60 * 60 * 24 * 30;
            setcookie("remember", $_POST["id"],  $time, "museen/");
        }
    }
}

/*----------------$_SESSION setters----------------*/

function set_session_errors(array $errors)
{
    unset($_SESSION["errors"]);
    $_SESSION["errors"] = $errors;
}

function set_session_user(string $id): void
{
    unset($_SESSION["user"]);
    $_SESSION["user"] = $id;
}

function set_session_authkey(string $authkey): void
{
    unset($_SESSION["authkey"]);
    $_SESSION["authkey"] = $authkey;
}

/*----------------Utility----------------*/
/**
 * Queries the database to check if visitor preferences have already been stored.
 *
 * @param string $id
 * @return void
 */
function are_set_movement_preferences(string $id)
{
    $are_set = false;
    $query = "SELECT amp_artistic_movement_id FROM artistic_movement_preference WHERE amp_visitor_id = '" . $id . "';";
    if (($result = make_query($query)) != FALSE) {
        $are_set = (pg_num_rows($result) >  0);
    }
    return $are_set;
}

function is_signed_in(): bool
{
    return isset($_SESSION["user"]);
}

function check_sign_in_redirect(string $page = "signin.php"): void
{
    if (!is_signed_in()) {
        if (isset($_COOKIE["remember"])) {
            set_session_user($_COOKIE["remember"]);
        } else {
            header("location:" . $page);
        }
    }
}

function check_remember_cookie(): void
{
    if (!is_signed_in()) {
        if (isset($_COOKIE["remember"])) {
            set_session_user($_COOKIE["remember"]);
        }
    }
}

function custom_stylesheet(string $stylesheet = NULL): void
{
    if (isset($stylesheet)) {
        echo "<link rel=\"stylesheet\" href=\"" . $stylesheet . "\"/>\n";
    }
}

/**
 * Generates a random string using a naive approach, i.e. pick a random index of the charset.
 *
 * @param integer $length the desired lenght of the result string
 * @return string
 */
function generate_random_string(int $length = 10): string
{
    $charset = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $str = '';

    for ($i = 0; $i < $length; $i++) {
        $index = rand(0, strlen($charset) - 1);
        $str .= $charset[$index];
    }

    return $str;
}

/**
 * Checks if the length of a given string is smaller than the provided max_length
 *
 * @param string $str
 * @param integer $max_length
 * @return boolean
 * 
 */
function is_smaller_str(string $str, int $max_length = 64): bool
{
    return strlen($str) <= $max_length;
}

/**
 * Checks if the provided string meets the visitor_id format
 *
 * @param string $str
 * @return boolean
 */
function is_visitor_id(string $str): bool
{
    return (strlen($str) >= 4) && (strlen($str) <= 20);
}
