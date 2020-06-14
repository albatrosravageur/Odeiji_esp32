<?php 

require_once '../includes/DbOperations.php';

$response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(
		isset($_POST['name']) and 
			isset($_POST['date']) and 
				isset($_POST['desc'] )and 
				isset($_POST['Entreprises_id']))
		{
		//operate the data further 

		$db = new DbOperations(); 

		$result = $db->createMeeting( 	$_POST['name'],
									$_POST['date'],
									$_POST['desc'],
									$_POST['Entreprises_id']
								);						
		$response['id'] = $result;

	}else{
		$response['error'] = true; 
		$response['message'] = "Required fields are missing";
	}
}else{
	$response['error'] = true; 
	$response['message'] = "Invalid Request";
}

echo json_encode($response);