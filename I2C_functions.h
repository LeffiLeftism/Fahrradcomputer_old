String floatToString(float number, uint8_t decimal_length, uint8_t goalStringLength) {
  if(number < 0) {
    number *= -1;
  }
  uint8_t num_len = String(number, decimal_length).length();
  if(num_len == goalStringLength) {
    return String(number, decimal_length);
  } else if(num_len > goalStringLength) {
    String zeros;
    for(uint8_t n = 0; n < goalStringLength; n++) {
      zeros += "-"; 
    }
    return zeros;
  } else {
    uint8_t int_length = String(int(number)).length();
    uint8_t adjust = 1;
    String goalString;
    if(decimal_length == 0) {
      adjust = 0;
    }
    if((goalStringLength - int_length - decimal_length - adjust) > 0) {
      for(uint8_t n = 0; n < (goalStringLength - int_length - decimal_length - adjust); n++) {
        goalString += " ";
      }
      goalString += String(number, decimal_length);
      return goalString;
    } else {
      String zeros;
      for(uint8_t n = 0; n < goalStringLength; n++) {
        zeros += "-"; 
      }
      return zeros;
    }
  }
}