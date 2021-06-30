<?php
$db_host = "localhost";
$db_user = "root";
$db_passwd = "qw9708";
$db_name = "km2021";

// MySQL - DB 접속.

$conn = mysqli_connect($db_host,$db_user,$db_passwd,$db_name);

if (mysqli_connect_errno()){
    echo "MySQL 연결 오류: " . mysqli_connect_error(); exit;
}
else {
    echo "DB : \"$db_name\"에 접속 성공.<br/>";
    $name=$_GET["name"];
    $email=$_GET["email"];
    $date=date("Y.m.d H:i");
    $sql = "INSERT INTO NFC VALUES(1,$name,$email,$date)";
    $result = mysqli_query($conn, $sql);
    mysqli_close($conn);
}
