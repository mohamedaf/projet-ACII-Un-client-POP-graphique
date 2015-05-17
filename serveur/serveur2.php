<?php

function resetMails() {
  global $mails;
  $mails = array(
                 "Return-Path: <fourmaux@l2ti.univ-paris13.fr>\nDelivered-To: online.fr-boiteres2004@free.fr\nReceived: (qmail 23137 invoked from network); 17 Oct 2004 19:17:21 -0000\nReceived: from galilee.univ-paris13.fr (HELO lpl.univ-paris13.fr) (194.254.164.38)\n  by mrelay3-1.free.fr with SMTP; 17 Oct 2004 19:17:21 -0000\nReceived: from pirogue.l2ti.univ-paris13.fr (pirogue.l2ti.univ-paris13.fr [194.254.163.182])\n.by lpl.univ-paris13.fr (8.12.10/8.12.10) with ESMTP id i9HJHL6X027011;\n.Sun, 17 Oct 2004 21:17:21 +0200 (CEST)\nReceived: from pirogue.l2ti.univ-paris13.fr (localhost [127.0.0.1])\n.by pirogue.l2ti.univ-paris13.fr (8.12.3/8.12.3/Debian-7.1) with ESMTP id i9HJHLkI014924;\n.Sun, 17 Oct 2004 21:17:21 +0200\nReceived: (from fourmaux@localhost)\n.by pirogue.l2ti.univ-paris13.fr (8.12.3/8.12.3/Debian-7.1) id i9HJHL9Y014922;\n.Sun, 17 Oct 2004 21:17:21 +0200\nFrom: Olivier Fourmaux <fourmaux@l2ti.univ-paris13.fr>\nDate: Sun, 17 Oct 2004 21:17:21 +0200\nTo: boiteres2004@free.fr\nSubject: Test\nMessage-ID: <20041017191721.GA14860@L2TI.univ-paris13.fr>\nMime-Version: 1.0\nContent-Type: multipart/mixed; boundary=\"pf9I7BMVVzbSWLtt\"\nContent-Disposition: inline\nUser-Agent: Mutt/1.3.28i\n\n\n--pf9I7BMVVzbSWLtt\nContent-Type: text/plain; charset=us-ascii\nContent-Disposition: inline\n\nCeci est un message au format MIME avec une image jointe. \n\n--pf9I7BMVVzbSWLtt\nContent-Type: image/png\nContent-Disposition: attachment; filename=\"openlogo-nd-50.png\"\nContent-Transfer-Encoding: base64\n\niVBORw0KGgoAAAANSUhEUgAAADIAAAA9CAMAAADYt8pWAAAAM1BMVEX////HADbLEEPjf5r7\n7/Lqn7Pxv83VQGj43+bcYIL0z9ntr8DOIE/SMFzgcI7mj6fZUHXA+KyXAAAAAWJLR0QAiAUd\nSAAAAAlwSFlzAAALEgAACxIB0t1+/AAAAAd0SU1FB9IBAhYHHEeg4/UAAAJKSURBVHiclVZJ\ngoQgDETCjiD/f+0kgDabOtapGymSygaMzRDSQ2QWlFh8XMFEvhF40Jpb/05TaRtgzbOFiUDQ\nnKs7xsFXjIywJLn9lkCAmSGbz0lHrxAedCNqjETD0K5ZF/IyHsQdw472YfnFXx6v8uDOsLR6\nzrVjFZeG84ubrSLkXa5Pzn4umEcTnecyi1NFH3cPDMZiY0Yyxd8ZTGTXeNlm/mGDlVBbwXOR\n2j4UFxymfzDDgZKgssVJuYASo6b06ehgUD/IlREMKraXcgp2ea7ljQKz6QMpERPjKgPp6w9F\nG5Ws4ofCFnvbU5dr+WxXzO09g+nNswXocOKnWbyfizkjVIqdxdttPSGo3TQrGerFiy0Ne48f\n5aiUfgNMbR57LTCph7V4tF63puJfR1mrN1gPURUHJ0q/YKqflMGcV+yD2FOOwdNYKZEqTpYf\ng1q3bV0nhPKPRqnxpjg2BmhfiqGJWMINkxakVDNCh4uMtqMXvlKGIKsYxsFYRoxjpUzgHBs/\n373no2tUvPo8Rk01plJETmjqjMYy/53r5naJwCV25dlkhip3b4OITobBcSZ5pEpPcIDNMyB2\n2tLcx2g74G13jnY+XpfH7FnxP0kJiGOeV2XyLYrXW60TLKs6322zb0+Qv5H+b9BlyJ8fBCPy\nXRHf9zUQ+2KWvUB+F3NNqA+gSbC6Yp4gP6em3jevl18HEVZPkX9wPsohDk/f9GQ705x44eSO\n/Rg3ajdtvlWowzrg+8faye/C9QXzQEJL9ltWseQOzXcNN4/3P3OaDmqvPEXSAAAAAElFTkSu\nQmCC\n\n--pf9I7BMVVzbSWLtt--\n",
                 "Return-Path: <steve@example.com>\nX-Original-To: john@example.com\nDelivered-To: john@example.com\nReceived: from example.com (localhost [127.0.0.1])\n    by ... (Postfix) with ESMTP id 692DF379C7\n    for <john@example.com>; Fri, 18 May 2007 22:59:31 +0200 (CEST)\nMessage-Id: <...>\nDate: Fri, 18 May 2007 22:59:31 +0200 (CEST)\nFrom: steve@example.com\nTo: undisclosed-recipients:;\n\nHi John,\n\njust wanted to drop you a note.");

  return "+OK ";
}

function traiterUser($str) {
  global $user, $connecte;

  /* echo "traiterUser --".trim($str)."--\n"; */

  if($connecte) return "+ERR Already connected";

  $pseudo = trim($str);
  if($pseudo) {
    $user = $pseudo;
    return "+OK ";
  } else {
    return "+ERR empty";
  }
}

function traiterPass($str) {
  global $membres, $connecte, $user;

  /* echo "traiterPass --".$str."--\n"; */

  if(!$user) return "+ERR USER first";
  if($connecte) return "+ERR Already connected";

  if(isset($membres[$user]) && $membres[$user] === $str) {
    $connecte = true;
    return "+OK ";
  } else {
    $user = "";
    $connecte = false;
    return "+ERR Connection failed";
  }
}

function traiterList() {
  global $mails, $connecte;
  $reponse = "+OK \n";

  /* echo "traiterList ----\n"; */

  if(!$connecte) return "+ERR Authorization first";

  foreach($mails as $ind => $m) {
    $reponse .= "$ind " . strlen($m) . "\n";
  }

  return $reponse . ".";
}

function traiterQuit() {
  global $user, $connecte;

  /* echo "traiterQuit ----\n"; */

  /* if(!$connecte) return "+ERR Authorization first"; */

  $user = "";
  $connecte = "";

  return "+OK ";
}

function traiterRetr($arg) {
  global $user, $connecte, $mails;

  /* echo "traiterRetr --".$arg."--\n"; */

  if(!$connecte) return "+ERR Authorization first";

  $n = intval($arg);

  if(isset($mails[$n])) {
    $res = "+OK ".strlen($mails[$n])." octets\n" . $mails[$n];
    unset($mails[$n]);
    return $res . "\n.";
  } else {
    return "+ERR not existing";
  }
}

function traiterTop($arg1, $arg2) {
  global $connecte, $mails;

  /* echo "traiterTop --" . $arg1 . "--" . $arg1 . "--\n"; */

  if(!$connecte) return "+ERR Authorization first";

  $idMsg = intval($arg1);
  $nbLignes = intval($arg2);

  if(isset($mails[$idMsg])) {
    if($nbLignes == 0) {
      $tmp = preg_split("@\n\n@", $mails[$idMsg]);
      return "+OK \n" . $tmp[0] . "\n.";
    } else {
      $res = "+OK \n";
      $tmp = preg_split("@\n@", $mails[$idMsg]);
      for($i=0; $i<$nbLignes; $i++) {
        $res .= $tmp[$i] . "\n";
      }
      return $res . ".";

    }
  } else {
    return "+ERR not existing";
  }

}

/* int preg_match ( string $pattern , string $subject [, array &$matches [, int $flags = 0 [, int $offset = 0 ]]] )     */
/* int socket_write ( resource $socket , string $buffer [, int $length = 0 ] )       */
  /* string socket_read ( resource $socket , int $length [, int $type = PHP_BINARY_READ ] ) */

function justDo($sock, $msg) {
  echo "reçu : " . $msg . "\n";

  if(preg_match("@(USER|user)\s+(.+)@", $msg, $res)) {
    socket_write ($sock , traiterUser($res[2]) . "\n");

  } else if(preg_match("@(PASS|pass)\s+(.+)@", $msg, $res)) {
    socket_write ($sock , traiterPass($res[2]) . "\n");

  } else if(preg_match("@(LIST|list)\s*@", $msg, $res)) {
    socket_write ($sock , traiterList() . "\n");

  } else if(preg_match("@(TOP|top)\s+(\d+)\s+(\d+)\s*@", $msg, $res)) {
    socket_write ($sock , traiterTop($res[2], $res[3]) . "\n");

  } else if(preg_match("@(QUIT|quit)\s*@", $msg, $res)) {
    socket_write ($sock , traiterQuit() . "\n");
    return false;
  } else if(preg_match("@(RETR|retr)\s+(\d+)\s*@", $msg, $res)) {
    socket_write ($sock , traiterRetr($res[2]) . "\n");

  } else if(preg_match("@(RESET|reset)\s*@", $msg, $res)) {
    socket_write ($sock , resetMails() . "\n");

  } else {
    socket_write ($sock , "+ERR Unknown\n");

  }
}

function traiterClient($sock) {
  do {
    $msg = socket_read($sock, 4096, PHP_NORMAL_READ);
    $tab = preg_split("@\n\n@", $msg);
    foreach($tab as $m)
      if(trim($m)) {
        if(justDo($sock, rtrim($m, "\n\r")) === false) $msg = false;
      }
  } while($msg !== false);

  socket_close($sock);
}



/* MAIN */

global $user;
global $connecte;
global $membres;
global $mails;
resetMails();                   /* Initialiser les mails */
$membres = array("a@hotmail.fr" => "a",
                 "b@hotmail.fr" => "b",
                 "c@hotmail.fr" => "c",
                 "d@hotmail.fr" => "d");

$port = intval($argv[1]);
if(!isset($argv[0])){
  echo "erreur appel serveur : port\n";
  return;
}




  /* resource socket_create ( int $domain , int $type , int $protocol ) */
  $sockServ = socket_create ( AF_INET, SOCK_STREAM, 0);
  if(!$sockServ) {
    return;
  }

  /* bool socket_bind ( resource $socket , string $address [, int $port = 0 ] ) */
  if(!socket_bind ($sockServ, "127.0.0.1", $port)) {
    return;
  }

  /* bool socket_listen ( resource $socket [, int $backlog = 0 ] ) */
  if(!socket_listen ($sockServ, 1)) {
    return;
  }

while(true) {

  echo "en attente sur " . $port . "\n";

  /* resource socket_accept ( resource $socket ) */
  $sock = socket_accept($sockServ);

  echo "client reçu\n";

  traiterClient($sock);
  traiterQuit();
  /* break; */
}

echo "extinction\n";
socket_close($sockServ);
?>