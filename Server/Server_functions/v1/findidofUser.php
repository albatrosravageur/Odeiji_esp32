<?php 

require_once '../includes/DbOperations.php';

$response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(isset($_POST['username'])){
		$db = new DbOperations(); 
		$user = $db->getUserByUsername($_POST['username']);
		$response['id'] = $user['id'];
	}else{
		$response['error'] = true; 
		$response['message'] = "Required fields are missing";
	}
}

echo json_encode($response);