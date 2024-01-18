void setup() 
{
  pinmode(2 , output)
  pinmode(26 , output)
  pinmode(27 , output)
}

void loop() 
{
  digitalwrite(2 , high)
  delay(500)
  digitalwrite(26 , high)
  delay(500)
  digitalwrite(27 , high)
  delay(500)
}