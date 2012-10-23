<html>
<head>
	<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
	<style>
	   	/* Pop Up */
		.popup{
			position:absolute;
			width:700px;
			height:300px;
			top: 30%;
			left:50%;
			margin-left: -350px;
			padding-top:20px;
			background:#FFF;
			border:1px solid white;
			z-index:10;
			display: none;
		}

		/* Blur Layer*/
		.blurLayer{
			position:absolute;
			left: 0px;
			top: 0px;
			width:100%; 
			height:100%; 
			background-color:#000000; 
			filter:Alpha(opacity=30); 
			opacity:0.6; 
			-moz-opacity:0.3;
			z-index:10;
			display: none;
		}
	</style>
	<script type="text/javascript">
	   $(document).ready(function(){
		$("#shutdown_layer").click(function(e){
			$("#shutdown_layer").css("display", "none");
			$("#shutdown_popup").css("display", "none");
		});

		$("#main_table").click(function(e){
			$("#shutdown_layer").css("display", "inline");
			$("#shutdown_popup").css("display", "inline");
		});
	   })
	</script>
</head>
<body>

<?
   $result_arr = array();
   $fp = fopen("result.txt", "r") or die("File cannot open");

   while(!feof($fp)){
	$str = fgets($fp);
	if($str == "")
	   continue;
	array_push($result_arr, $str);
   }
   fclose($fp);

   $numOfResult = count($result_arr);

   // display the result
   // the recent result will be displayed on the top
   // reverse order


   echo "<br><br><br>
	<center><font size='70'><b>VORONOI&nbsp;&nbsp;&nbsp;&nbsp;SCORE</b></font></center>
	<br><br><br><br><br>";     
   
   $line1 = explode(" ", $result_arr[$numOfResult-2]);
   $line2 = explode(" ", $result_arr[$numOfResult-1]);
   $team1_name = $line1[0];
   $team2_name = $line1[3];
   $team1_score = $line1[1] + $line2[4];
   $team2_score = $line1[4] + $line2[1];
   $team1_time = (int)(($line1[2] + $line2[5])/1000);
   $team2_time = (int)(($line1[5] + $line2[2])/1000);

   $winner="";
   if($team1_score > $team2_score)
      $winner = '<font size="10">'.$team1_name.'</font>';
   else if($team1_score < $team2_score)
      $winner = '<font size="10">'.$team2_name.'</font>';
   else
      $winner = '<font size="10">Tie Score!!!</font>';

   echo "<div id='main_table'>";
   echo "<table id='main_table' align='center' width='800' border='5' bordercolor='black'>
	         <tr align='center' height='30'>
		    <td width='400'>
		    	<br>
		    	<font size='10'>
			      ".$team1_name."
			</font>
			<br><br>
		    </td>
		    <td width='400'>
		    	<br>
		    	<font size='10'>
			      ".$team2_name."
			</font>
			<br><br>
		    </td>
		 </tr>
		 <tr align='center' height='80'>
		    <td>
			<br><br><br>
			<font size='10'>".number_format($team1_score)."</font><br><br>
		    	<font size='5'>".(int)($team1_time/60)."(m) ".(int)($team1_time%60)."(s)</font>
			<br><br><br>
		    </td>
		    <td>
			<br><br><br>
			<font size='10'>".number_format($team2_score)."</font><br><br>
		    	<font size='5'>".(int)($team1_time/60)."(m) ".(int)($team1_time%60)."(s)</font>
			<br><br><br>
		    </td>
		 </tr>
	      </table>
	      <br><br>";
	echo "</div>";
?>

	<div id="shutdown_layer" class="blurLayer"></div>
	<div id="shutdown_popup" class="popup">
	     <center>
		<br><br>	     
	   	<font size="30" color="blue"><b>Congraturation!!!!</b></font><br><br><br><br>
	        <b><?=$winner?></b>
		
	     </center>
	</div>

</body>
</html>
