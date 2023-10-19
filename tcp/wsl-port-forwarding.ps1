# From: https://stackoverflow.com/a/71546540

$ports=@(12345) # the ports you want to open
$addr='0.0.0.0';

$wslIP = bash.exe -c "hostname -I"
$found = $wslIP -match '\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}';

if(! $wslIP -match '\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}') {
  echo "WSL's IP cannot be found. Aborting";
  exit;
}

$portsStr = $ports -join ",";
iex "Remove-NetFireWallRule -DisplayName 'WSL 2 Firewall Unlock' ";
iex "New-NetFireWallRule -DisplayName 'WSL 2 Firewall Unlock' -Direction Outbound -LocalPort $portsStr -Action Allow -Protocol TCP";
iex "New-NetFireWallRule -DisplayName 'WSL 2 Firewall Unlock' -Direction Inbound -LocalPort $portsStr -Action Allow -Protocol TCP";

for ($i = 0; $i -lt $ports.length; $i++) {
  $port = $ports[$i];
  iex "netsh interface portproxy delete v4tov4 listenport=$port listenaddress=$addr";
  iex "netsh interface portproxy add v4tov4 listenport=$port listenaddress=$addr connectport=$port connectaddress=$wslIP";
}