<?php

//request User data from database

include_once("requester.php");

$action = $_REQUEST["action"];

$requester = new Requester;
$allData = (object) [];

if ($action == "register") {
    $email = $_REQUEST["email"];
    $password = $_REQUEST["password"];

    $count = $requester->checkUser($email);

    // create new user
    if ($count == 0) {
        $id = $requester->registerUser($email, $password);
        if ($id > 0) {
            $allData->result = true;
            $allData->userId = $id;
        }
    } else {
        $allData->alreadyRegistered = true;
        $allData->result = false;
    }
}

if ($action == "validate") {
    $code = $_REQUEST["code"];
}

print json_encode($allData, JSON_UNESCAPED_UNICODE);

?>