<?php 
 
	/*
	* Created by Belal Khan
	* website: www.simplifiedcoding.net 
	* Retrieve Data From MySQL Database in Android
	*/
	
	//database constants
	define('DB_HOST', 'localhost');
	define('DB_USER', 'root');
	define('DB_PASS', '');
	define('DB_NAME', 'odj2');
	
	//connecting to database and getting the connection object
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	
	//Checking if any error occured while connecting
	if (mysqli_connect_errno()) {
		echo "Failed to connect to MySQL: " . mysqli_connect_error();
		die();
    }
    
    $response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(
		isset($_POST['id']) and 
			isset($_POST['Entreprises_id']))
		{
            $stmt = $conn->prepare("UPDATE employees SET Entreprises_id=? WHERE id=?");
            $stmt->bind_param("ss",$_POST['Entreprises_id'],$_POST['id']);
            $stmt->execute();
            $response['error'] = false; 
            $response['message'] = "Entreprises changed";
        }
}else{
        $response['error'] = true; 
        $response['message'] = "Invalid Request";
}
    
    echo json_encode($response);
	
