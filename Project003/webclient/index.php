<!DOCTYPE HTML>
<html>
	<head>
		<title>Site!</title>
		<link rel="icon" href="resources/images/icon_browser.png" sizes="32x32">
		<meta charset="utf-8" />
		<link rel="stylesheet" href="resources/css/main.css" />
	</head>

	<body  onload="loadInstances()">
		<h1>Frontend Instance ID</h1>
		<h2 id="h1-front-instanceid">Loading...</h2>
		<p>Refresh page to check frontend host balancer (if active). Different IDs, different hosts!</p>

		<hr>

		<h1>Backend Instance ID</h1>
		<h2 id="h2-back-instanceid">Loading...</h2>
		<p>Click button to check backend host balancer (if active). Different IDs, different hosts!</p>

		<button type="button" onclick="refreshInstance('get-aws-backend-information.php', 'h2-back-instanceid')">Refresh!</button>

		<script>
			function loadInstances() {
				//Instance of frontend server (where this php is hosted) is reached by simply calling AWS meta-data address
				refreshInstance('get-aws-frontend-information.php', 'h1-front-instanceid');
				
				//Instance of backend server is reached by calling backend service for instance retrieval
				refreshInstance('get-aws-backend-information.php', 'h2-back-instanceid');
			}
			
			function refreshInstance(url, htmlElement) {
				document.getElementById(htmlElement).innerHTML = 'Loading...';
				var xmlhttp = new XMLHttpRequest();
				xmlhttp.onreadystatechange = function() {
					if (this.readyState == 4) {
						if (this.status == 200) {
							var jsonResponseObj = JSON.parse(this.responseText);
							if ( jsonResponseObj.stat == false ) {
								document.getElementById(htmlElement).classList.add('hx-for-error-msg');
							} else {
								document.getElementById(htmlElement).classList.remove('hx-for-error-msg');
							}
							document.getElementById(htmlElement).innerHTML = jsonResponseObj.msg;
						} else {
							document.getElementById(htmlElement).classList.add('hx-for-error-msg');
							document.getElementById(htmlElement).innerHTML = 'Error status ' + this.status + "!";
						}
					}
				};
				xmlhttp.open('GET', url, true);
				xmlhttp.send();
			}
		</script>
	</body>
</html>