<?php

/* �W�١GhiAir SMS2 For PHP��Class
 * ���g�� : HiNet - hiAir , Chih-Ming Liao
 * ��� : 2006/06/27
 * �ק�� : HiNet - hiAir , Mike
 * �ק��� : 2020/09/24
 * �Ƶ� : 
 */

class sms2{

   var $usenet_handle;    /* socket handle*/
   var $ret_code;
   var $ret_msg;
   var $send_msisdn="";
   var $send_msg_len=266; /* Socket �ǰe SendMsg �����׬�266 */
   var $ret_msg_len=244;  /* Socket ���� RetMsg �����׬�244 */
   var $send_set_len=100;
   var $send_content_len=160;
   var $ENCODING_BIG5=1;
   var $ENCODING_UCS2=3;
   var $ENCODING_UTF8=4;
   var $encoding_num= 1; #1:BIG5, 3:UCS-2, 4:UTF-8
   
   //function sms2(){}
   function sms2($encoding_num = null){ 
      if($encoding_num==$this->ENCODING_BIG5){
          $this->encoding_num = $this->ENCODING_BIG5;
      }elseif($encoding_num==$this->ENCODING_UTF8) {
          $this->encoding_num = $this->ENCODING_UTF8;
      }else{
          $this->encoding_num = $this->ENCODING_UCS2;
      }
   }

   /* �禡�����G�إ߳s�u�P�{��
    * $server_ip:���A��IP, $server_port:���A��Port, $TimeOut:�s�utimeout�ɶ�
    * $user_acc:�b��, $user_pwd:�K�X
    * return -1�G�����s�u����, 0�G�s�u�P�{�Ҧ��\, 1:�s�u���\�A�{�ҥ���
    */
   function create_conn($server_ip, $server_port, $TimeOut, $user_acc, $user_pwd){
      $msg_type=0;	   /* 0:�ˬd�b���K�X 1:�ǰe²�T 2:�d�߶ǰe���G */

      $this->usenet_handle = fsockopen($server_ip, $server_port, $errno, $errstr, $TimeOut);
      if(!$this->usenet_handle) {
      	 $this->ret_code=-1;
      	 $this->ret_msg="Connection failed!";
      	 return $this->ret_code;
      }
      /* �b���K�X�ˬd */
      $msg_set=$user_acc . "\0" . $user_pwd . "\0";
      $in_temp = pack("C",$msg_type) . pack("C",$this->encoding_num) . pack("C",1) . pack("C",0) . pack("C",strlen($msg_set)) . pack("C",0) . $msg_set;
      
      /*---�N����$send_msg_len����ƶ�\0�ɺ� */
      $len_p = $this->send_msg_len - strlen($in_temp);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      
      $in = $in_temp . $zero_buf;
      $out = '';
      $write = fwrite($this->usenet_handle, $in);
      $out = fread($this->usenet_handle, $this->ret_msg_len);
      /* ���Xret_code */
      $ret_C = substr($out, 0, 1);           /* ���X ret_code */
      $ret_code_array = unpack("C", $ret_C); /* �N$ret_C �নunsigned char , unpack �|return array*/
      $ret_code_value = $ret_code_array[1];    /* array[1]��ret_code���� */
      /* ���Xret_content*/
      $ret_CL = substr($out, 3, 1);          /* ���X ret_content_len */
      $ret_cl_array = unpack("C", $ret_CL);  /* �N$ret_CL �নunsigned char , unpack �|return array*/
      $ret_content_len = $ret_cl_array[1]; /* array[1]��ret_content_len���� */
      $ret_content = substr($out, 84, $ret_content_len); /* ���o�^�Ǫ����e*/

      $this->ret_code=$ret_code_value;  /* array[1]��ret_code���� */
      $this->ret_msg=$ret_content;
      return $this->ret_code;
   }   

   /* �禡�����G�ǰe��r²�T
    * $tel:��������, ²�T���e
    * return ret_code
    */
   // function send_text($mobile_number, $message){
   function send_text($mobile_number, $send_transType = 1, $send_orderTime = "", $send_limitTime = 1440, $message){
   	if(substr($mobile_number, 0, 1)== "+"){
      	 $msg_type=15; /* �ǰe���²�T */
      }else{
      	 $msg_type=1; /* �ǰe�ꤺ²�T */
      }

      //�غcmsg_set
      $msg_set_str = $mobile_number . "\0" . sprintf("%02d", $send_transType) . "\0";
      if($send_transType == 1){
         //�ߧY
      }else if( $send_transType == 2){
         //�ߧY+���e�O��
         $msg_set_str = $msg_set_str . sprintf("%04d", $send_limitTime) . "\0";
      }else if( $send_transType == 3){
         //�w��
         $msg_set_str = $msg_set_str . $send_orderTime . "\0";
      }else if( $send_transType == 4){
         //�w��+���e�O��
         $msg_set_str = $msg_set_str . $send_orderTime . "\0" . sprintf("%04d", $send_limitTime) . "\0";
      }

      /*---�N����$msg_set���ת���ƶ�\0�ɺ� */
      $len_p = $this->send_set_len - strlen($msg_set_str);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      $msg_set = $msg_set_str . $zero_buf;
   

      /*---�N����$msg_content���ת���ƶ�\0�ɺ� */
      $len_p = $this->send_content_len - strlen($message);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      $msg_content = $message . $zero_buf;
         
      $in = pack("C",$msg_type) . pack("C",$this->encoding_num) . pack("C",1) . pack("C",0) . pack("C",strlen($msg_set_str)) . pack("C",strlen($message)) . $msg_set . $msg_content;
      
      $write = fwrite($this->usenet_handle, $in);
      $out = fread($this->usenet_handle, $this->ret_msg_len);
      $ret_C = substr($out, 0, 1); /* ���X ret_code */
      $ret_code_array = unpack("C", $ret_C); /* �N$ret_C �নunsigned char , unpack �|return array*/
      $ret_code_value = $ret_code_array[1];    /* array[1]��ret_code���� */
   
      $ret_CL = substr($out, 3, 1); /* ���X ret_content_len */
      $ret_cl_array = unpack("C", $ret_CL); /* �N$ret_CL �নunsigned char , unpack �|return array*/
      $ret_content_len = $ret_cl_array[1]; /* array[1]��ret_content_len���� */
      $ret_content = substr($out, 84, $ret_content_len); /* ���o�^�Ǫ����e*/

      $this->ret_code=$ret_code_value;  /* array[1]��ret_code���� */
      $this->ret_msg=$ret_content;
      return $this->ret_code;
   }

	/*�o�e��²�T
	*/
	function send_long($mobile_number, $send_transType = 1 , $send_orderTime, $send_limitTime = 1440, $message, $encodeFrom="UTF-8"){
      //��²�T�C�h�r�ƭ���, ASCII:153, UCS2:67
      $msg_size_limit = 0;

      //�r�ժ���
      $words_length = mb_strlen($message, $encodeFrom);
      //����UTF-8����(�줸�ժ���)
      //mb_convert_encoding(message,encodeTo,encodeFrom), encodeTo:�ϥέ��ؽs�X�ǰe, encodeFrom:²�T���e�s�X�Ψt�����ҽs�X
      $utf8_bytes = mb_convert_encoding($message,"UTF-8",$encodeFrom);
      $utf8_bytes_len = strlen($utf8_bytes);
      // echo "words_length(strlen): ".$words_length."<br>\n";
      // echo "utf8_bytes_len(strlen): ".$utf8_bytes_len."<br>\n";

      //�p�G��̬ۦP�A���ܯ­^��
      if($utf8_bytes_len == $words_length){
			//�p�Gutf8 bytes���׸��l���e���׬۵��A���­^�Ʀr²�T
			//�­^�Ʀr�A���HBIG5�ǰe�A�~��C�h153
         $msg_size_limit = 153;
         $coding = 1;
         $encoding = "BIG5";
         // echo "�­^�Ʀr�A�HBIG5�e�X�C<br>\n";
      }else{
         //�D�­^�Ʀr�A���HUCS2�ǰe�A�~��C�h67
         $msg_size_limit = 67;
         $coding = 3;
         $encoding = "UCS-2BE";
         // echo "�D�­^�Ʀr�A�HUCS2�e�X�C<br>\n";
      }
      //�d��:
      // $string="�Ѯ�ܦn";
      // echo "byte_length(strlen): ".strlen($string)."<br>\n";
      // echo "words_length(mb_strlen): ".mb_strlen( $string, "utf-8")."<br>\n";
      //���G:
      // byte_length(strlen): 12<br>
      // words_length(mb_strlen): 4<br>

      //�s�X�ഫ
      // $message = iconv($encodeFrom, $encoding,$message);
      $message = mb_convert_encoding($message,$encoding,$encodeFrom);
      $message_len = mb_strlen($message, $encoding);

		
		//�p��ݭn�o�e�h�ֳq���h
		$numOfMsg = ceil($message_len / $msg_size_limit);
		// echo "numOfMsg:".$numOfMsg."<BR>\n";
				 
		$in = "";
		
		for($cnt =0 ; $cnt <= $numOfMsg; $cnt++){
		
			if($cnt == 0){
				//�Ĥ@�q�A�ǰe��²�T��T
				if(substr($mobile_number, 0, 1) == "+"){
					$msg_type = 18;
				}else{
					$msg_type = 11;	
				}
				
				$msg_coding = $coding;
				$msg_priority = 1;
				$msg_country_code = 0;
				
				//�غcmsg_set
				$msg_set_str = $mobile_number . "\0" . sprintf("%02d", $send_transType) . "\0";
				if($send_transType == 1){
					//�ߧY
				}else if( $send_transType == 2){
					//�ߧY+���e�O��
					$msg_set_str = $msg_set_str . sprintf("%04d", $send_limitTime) . "\0";
				}else if( $send_transType == 3){
					//�w��
					$msg_set_str = $msg_set_str . $send_orderTime . "\0";
				}else if( $send_transType == 4){
					//�w��+���e�O��
					$msg_set_str = $msg_set_str . $send_orderTime . "\0" . sprintf("%04d", $send_limitTime) . "\0";
				}
				
				//�غcmsg_set�ø�\0
				$msg_len = strlen($msg_set_str);
				$len_p = $this->send_set_len - strlen($msg_set_str);
				$zero_buf = "";
				for($i = 0 ; $i<$len_p ; $i++){
					$zero_buf = $zero_buf . "\0";
				}
				$msg_set = $msg_set_str . $zero_buf;
				
				//�غcmsg_content,�Τ���A�����M��
				$msg_content = "\0";
				$msg_content_len = 0;
				$len_p = $this->send_content_len - strlen($msg_content);
				$zero_buf ="";
				for($i = 0 ; $i<$len_p ; $i++){
					$zero_buf = $zero_buf . "\0";
				}
				$msg_content =  $msg_content . $zero_buf;
				
				$in = pack("C", $msg_type).pack("C", $msg_coding).pack("C",$msg_priority).pack("C", $msg_country_code).pack("C", strlen($msg_set)).pack("C",$numOfMsg).$msg_set.$msg_content;
			}else{
				//�ĤG�q�H��A�ǰe��²�T���e
				$msg_type = $cnt;	//��ĴX�q�A�q1�}�l
				$msg_coding = $coding; //�s�X
				$msg_priority = 1; //�Τ���
				$msg_country_code = 0; //�Τ���
				$msg_set_len = 0;
				
				//�غcmsg_set�A�Τ���A������'\0'
				$msg_set_str = "\0";
				$msg_len = strlen($msg_set_str);
				$len_p = $this->send_set_len - strlen($msg_set_str);
				$zero_buf = "";
				for($i = 0 ; $i<$len_p ; $i++){
					$zero_buf = $zero_buf . "\0";
				}
				$msg_set = $msg_set_str . $zero_buf;
				
				
				//�غcmsg_content
            $msg_part_bytes = mb_substr($message, ( ($cnt-1) *$msg_size_limit), $msg_size_limit, $encoding);
            // echo "msg_part_bytes: ".$msg_part_bytes."<br>\n";
            
				$msg_content_len = strlen($msg_part_bytes);
            // echo "msg_content_len: ".$msg_content_len."<br>\n";
            $len_p = $this->send_content_len - $msg_content_len;
				$zero_buf = "";
				for($i = 0; $i<$len_p ; $i++){
					$zero_buf = $zero_buf . "\0";
				}
				$msg_content = $msg_part_bytes . $zero_buf;
				
				$in = pack("C", $msg_type) . pack("C", $msg_coding). pack("C", $msg_priority).pack("C", $msg_country_code).pack("C",$msg_set_len).pack("C",$msg_content_len).$msg_set.$msg_content;
			}
			
			$write = fwrite($this->usenet_handle, $in);
			$out = fread($this->usenet_handle, $this->ret_msg_len);
			$ret_C = substr($out, 0, 1); //���Xret_code
			$ret_code_array = unpack("C", $ret_C);
         $ret_code_value = $ret_code_array[1];    /* array[1]��ret_code���� */
			
			$ret_CL = substr($out, 3,1 );
			$ret_cl_array = unpack("C", $ret_CL);
			$ret_content_len = $ret_cl_array[1];
			$ret_content = substr($out, 84, $ret_content_len);
			
			$this->ret_code = $ret_code_value;
			$this->ret_msg = $ret_content;
			
			//�p�G�ǰe��������@�qreturn code������s�A�Y���}
			if($this->ret_code != 0){
				return $this->ret_code;
			}
		}
		return $this->ret_code;
	}
   

   /* �禡�����G�d��text�o�T���G
    * $messageid:�T��ID
    * return ret_code
    */
   function query_text($messageid){
      $msg_type=2; /* �d��text�ǰe���G */
      $msg_set=$messageid;
      $in_temp = pack("C",$msg_type) . pack("C",$this->encoding_num) . pack("C",1) . pack("C",0) . pack("C",strlen($msg_set)) . pack("C",0) . $msg_set;
      
      /*---�N����$send_msg_len����ƶ�\0�ɺ� */
      $len_p = $this->send_msg_len - strlen($in_temp);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      
      $in = $in_temp . $zero_buf;
      $out = '';
      $write = fwrite($this->usenet_handle, $in);
      $out = fread($this->usenet_handle, $this->ret_msg_len);
      $ret_C = substr($out, 0, 1); /* ���X ret_code */
      $ret_code_array = unpack("C", $ret_C); /* �N$ret_C �নunsigned char , unpack �|return array*/
      $ret_code_value = $ret_code_array[1]; /* array[1]��ret_code���� */
   
      $ret_CL = substr($out, 3, 1); /* ���X ret_content_len */
      $ret_cl_array = unpack("C", $ret_CL); /* �N$ret_CL �নunsigned char , unpack �|return array*/
      $ret_content_len = $ret_cl_array[1]; /* array[1]��ret_content_len���� */
      $ret_content = substr($out, 84, $ret_content_len); /* ���o�^�Ǫ����e*/
      
      $this->ret_code=$ret_code_value;  /* array[1]��ret_code���� */
      $this->ret_msg=$ret_content;
      return $this->ret_code;
   }
   
   /* �禡�����G�d�ߪ�²�T�o�T���G
    * $messageid:�T��ID
    * return ret_code
    */
   function query_long($messageid){
      $msg_type=12; /* �d��text�ǰe���G */
      $msg_set=$messageid;
      $in_temp = pack("C",$msg_type) . pack("C",$this->encoding_num) . pack("C",1) . pack("C",0) . pack("C",strlen($msg_set)) . pack("C",0) . $msg_set;
      
      /*---�N����$send_msg_len����ƶ�\0�ɺ� */
      $len_p = $this->send_msg_len - strlen($in_temp);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      
      $in = $in_temp . $zero_buf;
      $out = '';
      $write = fwrite($this->usenet_handle, $in);
      $out = fread($this->usenet_handle, $this->ret_msg_len);
      $ret_C = substr($out, 0, 1); /* ���X ret_code */
      $ret_code_array = unpack("C", $ret_C); /* �N$ret_C �নunsigned char , unpack �|return array*/
      $ret_code_value = $ret_code_array[1]; /* array[1]��ret_code���� */
   
      $ret_CL = substr($out, 3, 1); /* ���X ret_content_len */
      $ret_cl_array = unpack("C", $ret_CL); /* �N$ret_CL �নunsigned char , unpack �|return array*/
      $ret_content_len = $ret_cl_array[1]; /* array[1]��ret_content_len���� */
      $ret_content = substr($out, 84, $ret_content_len); /* ���o�^�Ǫ����e*/
      
      $this->ret_code=$ret_code_value;  /* array[1]��ret_code���� */
      $this->ret_msg=$ret_content;
      return $this->ret_code;
   }
   
   /* �禡�����G�����w���u²�T
    * $messageid:�T��ID
    * return ret_code
    */
   function cancel_text($messageid){
      $msg_type=16; /* �����w����r²�T */
      $msg_set=$messageid;
      $in_temp = pack("C",$msg_type) . pack("C",$this->encoding_num) . pack("C",1) . pack("C",0) . pack("C",strlen($msg_set)) . pack("C",0) . $msg_set;
      
      /*---�N����$send_msg_len����ƶ�\0�ɺ� */
      $len_p = $this->send_msg_len - strlen($in_temp);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      
      $in = $in_temp . $zero_buf;
      $out = '';
      $write = fwrite($this->usenet_handle, $in);
      $out = fread($this->usenet_handle, $this->ret_msg_len);
      $ret_C = substr($out, 0, 1); /* ���X ret_code */
      $ret_code_array = unpack("C", $ret_C); /* �N$ret_C �নunsigned char , unpack �|return array*/
      $ret_code_value = $ret_code_array[1]; /* array[1]��ret_code���� */
   
      $ret_CL = substr($out, 3, 1); /* ���X ret_content_len */
      $ret_cl_array = unpack("C", $ret_CL); /* �N$ret_CL �নunsigned char , unpack �|return array*/
      $ret_content_len = $ret_cl_array[1]; /* array[1]��ret_content_len���� */
      $ret_content = substr($out, 84, $ret_content_len); /* ���o�^�Ǫ����e*/
      
      $this->ret_code=$ret_code_value;  /* array[1]��ret_code���� */
      $this->ret_msg=$ret_content;
      return $this->ret_code;
   }

   /* �禡�����G�����w����²�T
    * $messageid:�T��ID
    * return ret_code
    */
   function cancel_long($messageid){
      $msg_type=17; /* �����w����r(�t���)��²�T */
      $msg_set=$messageid;
      $in_temp = pack("C",$msg_type) . pack("C",$this->encoding_num) . pack("C",1) . pack("C",0) . pack("C",strlen($msg_set)) . pack("C",0) . $msg_set;
      
      /*---�N����$send_msg_len����ƶ�\0�ɺ� */
      $len_p = $this->send_msg_len - strlen($in_temp);
      $zero_buf='';
      for($i=0;$i<$len_p;$i++){
         $zero_buf = $zero_buf . "\0";
      }
      
      $in = $in_temp . $zero_buf;
      $out = '';
      $write = fwrite($this->usenet_handle, $in);
      $out = fread($this->usenet_handle, $this->ret_msg_len);
      $ret_C = substr($out, 0, 1); /* ���X ret_code */
      $ret_code_array = unpack("C", $ret_C); /* �N$ret_C �নunsigned char , unpack �|return array*/
      $ret_code_value = $ret_code_array[1]; /* array[1]��ret_code���� */
   
      $ret_CL = substr($out, 3, 1); /* ���X ret_content_len */
      $ret_cl_array = unpack("C", $ret_CL); /* �N$ret_CL �নunsigned char , unpack �|return array*/
      $ret_content_len = $ret_cl_array[1]; /* array[1]��ret_content_len���� */
      $ret_content = substr($out, 84, $ret_content_len); /* ���o�^�Ǫ����e*/
      
      $this->ret_code=$ret_code_value;  /* array[1]��ret_code���� */
      $this->ret_msg=$ret_content;
      return $this->ret_code;
   } 

   /* �^��ret_content���� */
   function get_ret_msg(){
      return $this->ret_msg;
   }

   /* �^��send_tel���� */
   function get_send_tel(){
      return $this->send_msisdn;
   }
  
   /* �����s�u */
   function close_conn(){
   	  if($this->usenet_handle)
         fclose($this->usenet_handle);
   }
}
?>