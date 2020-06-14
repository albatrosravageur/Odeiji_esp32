<?php 

require_once '../includes/DbOperations.php';

$response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(isset($_POST['username'])){
		$db = new DbOperations(); 
        $user = $db->getUserByUsername($_POST['username']);
        $response['id'] = $user['id'];
        $response['token'] = $user['token'];
        $response['Entreprises_id'] = $user['Entreprises_id'];
        $response['email'] = $user['email'];
		$response['password'] = $user['password'];
	}else{
		$response['error'] = true; 
		$response['message'] = "Required fields are missing";
	}
}

echo json_encode($response);