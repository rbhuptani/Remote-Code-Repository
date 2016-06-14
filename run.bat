start "Client" .\Debug\Client.exe localhost:8083 localhost:8090
start "WPF Client" .\Debug\CppCli-WPF-App.exe localhost:8081 localhost:8090
start "Server" .\Debug\Server.exe localhost:8090
start "Test Executive" .\Debug\TestExec.exe