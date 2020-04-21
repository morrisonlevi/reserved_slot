<?php

interface LoggerInterface
{
    public function debug($message, array $context = array());
}

require __DIR__ . '/vendor/autoload.php';

$prophet = new \Prophecy\Prophet();
$logger = $prophet->prophesize('LoggerInterface');
$any = Prophecy\Argument::any();
$logger->debug($any)->shouldNotBeCalled();

