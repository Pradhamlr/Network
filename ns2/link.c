link-state.tcl
set ns [new Simulator]
$ns rtproto LS
set nf [open ls1.tr w]
$ns trace-all $nf
set nr [open ls2.nam w]
$ns namtrace-all $nr
proc finish {} {
 close $nr
 close $nf
 global ns nf nr
 $ns flush-trace
 exec nam ls2.nam
 exit 0
}
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n0 1Mb 10ms DropTail
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0
set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
set null0 [new Agent/Null]
$ns attach-agent $n3 $null0
set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set packetSize_ 500
$cbr1 set interval_ 0.005
set null1 [new Agent/Null]
$ns attach-agent $n3 $null1
$ns connect $udp0 $null0
$ns connect $udp1 $null1
$ns at .1 "$cbr1 start"
$ns at .2 "$cbr0 start"
$ns at 45.0 "$cbr1 stop"
$ns at 45.1 "$cbr0 stop"
$ns at 50.0 "finish"
$ns run
AWK Program for LS
BEGIN {
 print "Performance evaluation"
send = 0
 recv = 0
 dropped = 0
 rout = 0
}
{
 if ($1 == "+" && (($3 == "0") || ($3 == "1")) && $5 == "cbr") {
 send++
 }
 }
 if ($1 == "r" && $4 == "3" && $5 == "cbr") {
 recv++
 if ($1 == "d") {
 }
 dropped++
 if ($1 == "r" && $5 == "rtProtoLS") {
 rout++
 }
}
END {
 print "No of packets Send: " send
 print "No of packets Received: " recv
 print "No of packets dropped: " dropped
 NOH = rout / recv
 print "No of routing packets: " rout
 PDR = recv / send
 print "Normalised overhead: " NOH
 print "Packet delivery ratio: " PDR
}