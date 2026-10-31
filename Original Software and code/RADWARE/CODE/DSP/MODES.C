
calculate_modes()

{
/*  if (play_voice)
  {
    play_buffer[0] = 0;
    if (dspGetData(hAlg,(int *) data_buffer[buffer_write],GETDATASIZE)==0)
      Copy_Data_128(play_buffer,data_buffer[buffer_write]+1);
    else
      if (++buffer_write >1)
        buffer_write = 0;
    plotMsgData[0] = play_buffer[0];
    plotMsgData[1] = play_buffer[1];
    dspSendMsg (hApp, hAlg,msg_REQUEST_PLAY_DATA , 2 , plotMsgData);
  }
  if (NewSetTone>=0)
  {
    SetTone(NewSetTone);
    NewSetTone = NewSetTone*-1-1;
  }
  pBuf = (int *)wParam2;
  if (NewIFGain>=0 && NewIFGain<=15)
  {
    ChangeIFGain2((1+TalkOn)*0x4040+NewIFGain*0x101);
    CurrentIFGain = NewIFGain;
	 NewIFGain = -1;
    AnalogAGC = 0;
  }
  if (NewSampleRate>0)
  {
	 pSpregs = &Registers_1848;
	 if (NewSampleRate == 2)
		Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xF0F0)+0x0202; /* 0x0404 for 32 KHz 0x0707 for 22.5 KHz*/
/*    else
		Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xF0F0);
	 dspUtilProgram1848(pSpregs);
	 NewSampleRate *= -1;
  }
  if (SendingMorse==1)
  {
    FillSin(lastInputBuffer);
	 for(wTemp=0;wTemp<256;wTemp+=2)
	 {
      if (Morse[0]<=10)
		  lastInputBuffer[wTemp] = 0;
		lastInputBuffer[wTemp+1] = lastInputBuffer[wTemp];
		if (++MorsePosition>=DitLength)
		{
		  MorsePosition = 0;
        if (Morse[0]>10)
			 Morse[0]--;
        else
          if (Morse[0]<10 && Morse[0] != 0)
            Morse[0]++;
        if (Morse[0] == 10)
			 DeleteMorse();
      }
    }
  }
  if (SendingMorse == 2)
    FillSin(lastInputBuffer);
  if (TalkOn)
  {
    ClearError();
	 if (record_voice)
	 {
      Copy_Data_256(pBuf,lastInputBuffer);
      pBuf[0] = lastInputBuffer[0];
      Copy_Data_256_to_128(plotMsgData+4,pBuf); /*lastInputBuffer);*/
      dspSendData(hAlg,plotMsgData,134);
    }
    else
    {
      switch(mode)
      {
        case 0 : Copy_Data_256(pBuf,lastInputBuffer);
                 pBuf[0] = lastInputBuffer[0];
                 if (!SendingMorse)
                 {
                   Filter_60_Hz(pBuf);
                   if (AGC_On)
                     AGC(pBuf);
					  }
					  else
						 Filter_Both(pBuf);
					  MultiplyAllByVolume(pBuf);
					  break;
		  case 2 : for(wTemp=0; wTemp<256; wTemp+=2)
					  {
						 pBuf[wTemp+1] = sFilter(lastInputBuffer[wTemp]);
						 pBuf[wTemp] = delayLine(lastInputBuffer[wTemp]);
					  }
                 if (!SendingMorse)
					  {
						 Filter_60_Hz(pBuf);
						 if (AGC_On)
						   AGC(pBuf);
					  }
 					  Filter_Both(pBuf);
					  MultiplyAllByVolume(pBuf);
					  Adjust_I(pBuf);
					  break;
		  case 1 : for(wTemp=0; wTemp<256; wTemp+=2)
					  {
						 pBuf[wTemp] = sFilter(lastInputBuffer[wTemp]);
						 pBuf[wTemp+1] = delayLine(lastInputBuffer[wTemp]);
					  }
                 if (!SendingMorse)
					  {
						 Filter_60_Hz(pBuf);
						 if (AGC_On)
							AGC(pBuf);
					  }
 					  Filter_Both(pBuf);
					  MultiplyAllByVolume(pBuf);
					  Adjust_I(pBuf);
					  break;
		  case 3 : for(wTemp=0; wTemp<256; wTemp+=2)
					  {
						 hilbert = lastInputBuffer[wTemp];
						 delay = lastInputBuffer[wTemp];
						 pBuf[wTemp] = hilbert;
						 pBuf[wTemp+1] = delay;
					  }
					  if (!SendingMorse)
					  {
						 Filter_60_Hz(pBuf);
						 if (AGC_On)
							AGC(pBuf);
					  }
 					  Filter_Both(pBuf);
					  MultiplyAllByVolume(pBuf);
					  Adjust_I(pBuf);
					  break;
		  case 4 : for(wTemp=0; wTemp<256; wTemp+=2)
					  {
						 hilbert = lastInputBuffer[wTemp];
						 delay = lastInputBuffer[wTemp];
						 pBuf[wTemp] = hilbert;
						 pBuf[wTemp+1] = delay;
					  }
  					  Filter_60_Hz(pBuf);
					  if (AGC_On)
						 AGC(pBuf);
					  MultiplyAllByVolume(pBuf);
					  break;
		}
	 }
  }
  else
  {
    if (play_voice)
    {
      Copy_Data_128_to_256(lastInputBuffer,play_buffer);
    }
    if (PlotLSBT && SB_DATA == 0 && !record_voice && !play_voice)
	 {
		if (fft_time==2)
		{
		  pInt = (int *) LSB_BUFFER;
		  for (wTemp=0; wTemp < 256; wTemp++)
			 plotMsgData[wTemp]  = *pInt++;
		  dspSendMsg (hApp, hAlg,103 , SIZE_PLOTMSG, plotMsgData);
		}
	 }
	 ClearError();
	 count = SB_DATA;
	 switch(mode)
	 {
		case 0  : Hilbert_And_Delay(lastInputBuffer,pBuf);
				    Make_SSB(pBuf,&LSB_BUFFER[count]);
					 Make_SSB(pBuf+1,&USB_BUFFER[count]);
					 Filter_Both(pBuf);
					 Remove_USB(pBuf);
					 Which_AGC(pBuf);
					 MultiplyAllByVolume(pBuf);
					 Filter_60_Hz(pBuf);
					 count +=128;
					 plotMsgData[0] = Center(USB_BUFFER)/2-Center(LSB_BUFFER)/2;
					 if (!play_voice && !record_voice)
					   dspSendMsg (hApp, hAlg,msg_FREQ_CAL, SIZE_PLOTMSG, plotMsgData);
					 break;
		case 10 : Hilbert_And_Delay(lastInputBuffer,pBuf);
					 Make_SSB(pBuf,&LSB_BUFFER[count]);
					 Make_SSB(pBuf+1,&USB_BUFFER[count]);
					 Filter_Both(pBuf);
					 Remove_LSB(pBuf);
					 Which_AGC(pBuf);
					 MultiplyAllByVolume(pBuf);
					 Filter_60_Hz(pBuf);
					 count +=128;
					 plotMsgData[0] = Center(USB_BUFFER)/2-Center(LSB_BUFFER)/2;
					 if (!play_voice && !record_voice)
						dspSendMsg (hApp, hAlg,msg_FREQ_CAL, SIZE_PLOTMSG, plotMsgData);
					 break;
		case 2 :  Adjust_I(lastInputBuffer);
					 Hilbert_And_Delay(lastInputBuffer,pBuf);
					 Filter_Both(pBuf);
					 Make_SSB(lastInputBuffer,&LSB_BUFFER[count]);
					 Make_SSB(lastInputBuffer+1,&USB_BUFFER[count]);
					 Remove_USB(pBuf);
					 Filter_60_Hz(pBuf);
					 Which_AGC(pBuf);
					 MultiplyAllByVolume(pBuf);
                count +=128;
					 break;
		case 1 :  Adjust_I(lastInputBuffer);
                Hilbert_And_Delay(lastInputBuffer,pBuf);
					 Filter_Both(pBuf);
					 Make_SSB(pBuf,&LSB_BUFFER[count]);
					 Make_SSB(pBuf+1,&USB_BUFFER[count]);
					 Remove_LSB(pBuf);
					 Filter_60_Hz(pBuf);
					 Which_AGC(pBuf);
					 MultiplyAllByVolume(pBuf);
					 count +=128;
					 break;
		case 3 :  Adjust_I(lastInputBuffer);
					 Hilbert_And_Delay(lastInputBuffer,pBuf);
					 Which_AGC(pBuf);
					 MultiplyAllByVolume(pBuf);
					 Filter_Both(pBuf);
					 Make_SSB(pBuf,&LSB_BUFFER[count]);
					 Make_SSB(pBuf+1,&USB_BUFFER[count]);
					 Filter_60_Hz(pBuf);
					 count +=128;
					 break;
		case 4 :  Copy_Data_256(Temp_Buffer,lastInputBuffer);
					 AGC(Temp_Buffer);
					 plotMsgData[0]=GetVolume();
					 plotMsgData[1]=0;
					 if (--smeter_time == 0)
					 {
						smeter_time = 8;
						dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
					 }
					 vol = GetVolume()+32768;
					 RestoreVolume();
					 if (vol<squelch)
						Mute_Samples(pBuf);
					 else
					 {
                  Highpass_Both(Temp_Buffer);
						FM2(Temp_Buffer,pBuf);
						MultiplyAllByVolume(pBuf);
	               Decimate_And_Filter(pBuf);
						Make_SSB_FM(pBuf+1,&LSB_BUFFER[count]);
					 }
					 count +=128;
					 break;
		case 6 :  Copy_Data_256(Temp_Buffer,lastInputBuffer);
					 AGC(Temp_Buffer);
					 plotMsgData[0]=GetVolume();
					 plotMsgData[1]=0;
					 if (--smeter_time == 0)
					 {
						smeter_time = 4;
						dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
					 }
					 vol = GetVolume()+32768;
					 RestoreVolume();
					 if (vol<squelch)
					 {
                  Mute_Samples(pBuf);
						Make_SSB(pBuf,&USB_BUFFER[count]);
						Make_SSB(pBuf+1,&LSB_BUFFER[count]);
					 }
					 else
					 {
						Highpass_Both(Temp_Buffer);
						FM2(Temp_Buffer,pBuf);
						MultiplyAllByVolume(pBuf);
                  FM_RTTY(pBuf);
                  dspSendMsg(hApp,hAlg,msg_RTTY,
                             Convert_RTTY_To_Bits(pBuf+1,plotMsgData),
                             plotMsgData);
						Make_SSB(pBuf+1,&LSB_BUFFER[count]);
					 }
					 count +=128;
					 break;
		case 7 :  Copy_Data_256(Temp_Buffer,lastInputBuffer);
					 AGC(Temp_Buffer);
					 plotMsgData[0]=GetVolume();
					 plotMsgData[1]=0;
					 if (--smeter_time == 0)
					 {
						smeter_time = 4;
						dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
					 }
					 vol = GetVolume()+32768;
					 RestoreVolume();
					 if (vol<squelch)
					 {
                  Mute_Samples(pBuf);
						Make_SSB(pBuf,&USB_BUFFER[count]);
						Make_SSB(pBuf+1,&LSB_BUFFER[count]);
					 }
					 else
					 {
						Highpass_Both(Temp_Buffer);
						PSK(Temp_Buffer,pBuf);
						MultiplyAllByVolume(pBuf);
                  FM_RTTY(pBuf);
						Make_SSB(pBuf+1,&LSB_BUFFER[count]);
					 }
					 count +=128;
					 break;
	 }
/*	 if (record_voice)
	 {
	   int which = 0;
      for(wTemp=0;wTemp<128;wTemp++);
		  plotMsgData[wTemp] = wTemp;
		dspSendMsg(hApp,hAlg,msg_VOICE_DATA,128,plotMsgData);
	 }
	 else
	 {
		if (!play_voice)
		  if ((mode !=4 && SB_DATA == 128) || (mode ==4 && SB_DATA == 192))
		  {
			 if (fft_time==3 && PlotUSBT)
			 {
			   pInt = (int *) USB_BUFFER;
				for (wTemp=0; wTemp < 256; wTemp++)
				  plotMsgData[wTemp]  = *pInt++;
				dspSendMsg (hApp, hAlg,104 , SIZE_PLOTMSG, plotMsgData);
			 }
			 if (fft_time == 2 && PlotUSBS)
			 {
				pInt = (int *) dousbfft();
				for (wTemp=0; wTemp < SIZE_PLOTMSG; wTemp++)
				  plotMsgData[wTemp]  = *pInt++;
				dspSendMsg (hApp, hAlg,101 , SIZE_PLOTMSG, plotMsgData);
			 }
			 if (--fft_time==0)
			 {
				fft_time = 26;
				if (PlotLSBS)
				{
				  pInt = (int *) dolsbfft();
				  for (wTemp=0; wTemp < SIZE_PLOTMSG; wTemp++)
					 plotMsgData[wTemp]  = *pInt++;
				  dspSendMsg (hApp, hAlg,102 , SIZE_PLOTMSG, plotMsgData);
				}
			 }
		  }
		  if (mode != 4)
			 SB_DATA = 128 - SB_DATA;
		  else
			 if ((SB_DATA+=64)>192)
				SB_DATA = 0;
		}
*/	 }
    wTemp = dspSendIo(hAlg, OutputDevice,pBuf, SIZE_IO_BUFFER);
/*	 plotMsgData[2] = 0;
	 if (AGC_On != 0 && !TalkOn)
	 {
		if (AnalogAGC<-1)
		  if (CurrentIFGain == 0)
			 plotMsgData[2] = -1;
        else
          NewIFGain=CurrentIFGain-1;
      if (AnalogAGC>64)
        if (CurrentIFGain == 15)
          plotMsgData[2] = 1;
        else
			 NewIFGain=CurrentIFGain+1;
    }
	 if ((NewIFGain>=0 || plotMsgData[2]!=0))
    {
      plotMsgData[0] = NewIFGain;
      plotMsgData[1] = GetVolume();
      AnalogAGC = 0;
		dspSendMsg (hApp, hAlg,MSG_AGC_ANALOG , SIZE_PLOTMSG, plotMsgData);
  	 }
    if (wTemp != DSPERR_SUCCESS)
	 {
		SendErrCount++;
		SendLastErr = wTemp;
	 }
	 break;
*/  }
}
