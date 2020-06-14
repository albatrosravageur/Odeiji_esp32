<?php 

require_once '../includes/DbOperations.php';

$response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(isset($_POST['username']) and isset($_POST['password'])){
		$db = new DbOperations(); 

		if($db->userLogin($_POST['username'], $_POST['password'])){
			/*$user = $db->getUserByUsername($_POST['username']);
			$response['error'] = false; 
			$response['id'] = $user['id'];
			$response['email'] = $user['email'];
			$response['username'] = $user['username'];*/
			echo "Data Matched";
		}else{
			/*$response['error'] = true; 
			$response['message'] = "Invalid username or password";		*/	
			echo "Invalid Username or Password Please Try Again";
		}

	}else{
		/*$response['error'] = true; 
		$response['message'] = "Required fields are missing";*/
		echo "Check Again";
	}
}

//echo json_encode($response);