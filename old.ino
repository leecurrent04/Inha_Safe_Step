
    if (currentTime - prevTime >= interval)
    {

        Serial.print(String(count) + ", ");
        count = 0;
        prevTime = currentTime;  // 이전 초기화 시간 업데이트
    }
    
    

        // If the FSR has no pressure, the resistance will be near infinite.
        // So the voltage should be near 0.
        // If the analog reading is non-zero
        if (sum != 0)
        {
            float voltage_arr[pin_length];
            // Use ADC reading to calculate voltage:
            for (int i = 0; i < pin_length; ++i)
            {
                voltage_arr[i] = ADC_arr[i] * VCC / 1023.0;
            }

            // Use voltage and static resistor value to calculate FSR resistance:
            float resistance_arr[pin_length];
            for (int i = 0; i < pin_length; ++i)
            {
                resistance_arr[i] = R_DIV * (VCC / voltage_arr[i] - 1.0);
            }

            print_db(resistance_arr);
            state_over_people = over_two(resistance_arr, fst_target_arr, snd_target_arr);
            Serial.println(state_over_people);
        }




        //delay(interval);
        bool isSafe = (state_helmet_on && !state_over_people);  // Check for safe conditions

        if (count == 0)
        {  // Speed is 0 for 2 seconds
            if (isSafe)
            {
                digitalWrite(9, LOW);  // Release the brake
                noTone(5);             // Turn off the buzzer
            }
            else
            {
                digitalWrite(9, HIGH);  // Apply the brake
                delay(3000);
                digitalWrite(9, LOW);  // Stop the brake
                noTone(5);             // Turn off the buzzer
            }
        }
        else
        {  // Speed is not 0
            if (isSafe)
            {
                digitalWrite(9, LOW);  // Release the brake
                noTone(5);             // Turn off the buzzer
            }
            else
            {
                digitalWrite(9, HIGH);  // Release the brake
                tone(5, 1000);          // Turn on the buzzer
            }
        }