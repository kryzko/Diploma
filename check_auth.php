<?php
session_start();
if (!isset($_SESSION['user_id'])) {
    header("Location: main.php");
    exit;
}

$db = new mysqli('localhost', 'root', '', 'diploma');
$user_id = $_SESSION['user_id'];
$result = $db->query("SELECT email FROM users WHERE userID = $user_id");
$user = $result->fetch_assoc();
$db->close();

$email = htmlspecialchars($user['email']);
include 'control.html';
?>