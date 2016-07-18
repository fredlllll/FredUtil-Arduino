
class FredThermistor{
	const int R_resistor, R_thermistor;
	const bool thermIsPullup;
	const uint32_t beta;
	public:
	FredThermistor(const int R_resistor = 10000, const int R_thermistor = 10000, const bool thermIsPullup = true, const uint32_t beta = 3800) 
	: R_resistor(R_resistor), R_thermistor(R_thermistor), thermIsPullup(thermIsPullup), beta(beta){
		
	}
	
	double temperatureFromRawADC(const int rawADC, const uint8_t adcBits = 10) const{
		union { //cleaner reuse of variables
			double adc;
			double resistance;
			double temp;
		};
		
		adc = rawADC / (double)((1<<adcBits)-1);
		if(thermIsPullup){ //also used wolfram alpha to get these formulas using the voltage divider formula
			resistance = (R_resistor / adc) - R_resistor;//therm pullup
		}else{
			resistance = -((adc * R_resistor) / (adc - 1));//resistor pullup
		}
		
		//wolfram: solve 1/t = 1/298.15+1/B ln(r/R) for t
		//the 298.15 is 25C in Kelvin. formula taken from wikipedia https://en.wikipedia.org/wiki/Thermistor#B_or_.CE.B2_parameter_equation and solved with wolfram alpha
		//where t = T, r = R, and R = R0
		temp = (5963*beta)/(20*beta+5963*log(resistance/R_thermistor)); //hope that stuff with the ints works out(used a 32bit int for beta, so we should be fine). probably faster than using floats for everything
		temp = temp - 273.15;  // Convert Kelvin to Celsius
		return temp;
	}
};