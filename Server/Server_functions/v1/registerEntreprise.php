<?php 

require_once '../includes/DbOperations.php';

$response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(
		isset($_POST['Name']) )
		{
		//operate the data further 

		$db = new DbOperations(); 

		$result = $db->createEntreprise( $_POST['Name']);
		if($result == 1){
			$response['error'] = false; 
			$response['message'] = "Entreprise registered successfully";
		}elseif($result == 2){
			$response['error'] = true; 
			$response['message'] = "Some error occurred please try again";			
		}

	}else{
		$response['error'] = true; 
		$response['message'] = "Required fields are missing";
	}
}else{
	$response['error'] = true; 
	$response['message'] = "Invalid Request";
}

echo json_encode($response);