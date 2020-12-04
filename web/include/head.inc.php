<?php
require_once "include/functions.inc.php";
?>

<!DOCTYPE html>
<html lang="fr">

<head>
    <meta charset="utf-8" />
    <meta name="author" content="Adel Abbas, Aelien Moubeche" />
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
    <meta name="date" content=<?php echo "\"" . date('D, d M Y') . "\"" ?> />
    <meta name="keywords" content="Museum, art, visite guidée, museen, <?php echo $title ?>" />
    <meta name="description" content="" />
    <title><?php echo $title ?></title>

    <!-- Bootstrap core CSS -->
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous" />
    <?php
        $stylesheet = !isset($stylesheet)? null : $stylesheet;
        custom_stylesheet($stylesheet);
    ?>
</head>