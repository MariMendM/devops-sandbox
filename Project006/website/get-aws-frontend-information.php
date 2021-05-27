<?php
//AWS meta-data address does not change; each instance reaches information using always same url
$textResponse = file_get_contents("http://169.254.169.254/latest/meta-data/instance-id");
if ( $textResponse == false ) {
    echo json_encode( array('stat'=>false, 'msg'=>'Error requesting service!') );
    return;
}

echo json_encode( array('stat'=>true, 'msg'=>$textResponse) );
?>