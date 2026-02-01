<?php
session_start();
$db = new mysqli('localhost', 'root', '', 'diploma');

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $email = $_POST['email'] ?? '';
    $password = $_POST['password'] ?? '';

    $stmt = $db->prepare("SELECT userID, password FROM users WHERE email = ?");
    $stmt->bind_param("s", $email);
    $stmt->execute();
    $result = $stmt->get_result();
    $user = $result->fetch_assoc();

    if ($user && $password === $user['password']) {
        $_SESSION['user_id'] = $user['userID'];
        header("Location: check_auth.php");
        exit;
    } else {
        $_SESSION['login_error'] = "Неверный email или пароль";
        $_SESSION['old_email'] = htmlspecialchars($email); // Сохраняем введенный email
        header("Location: index.php");
        exit;
    }
}
?>