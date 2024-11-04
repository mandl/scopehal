/***********************************************************************************************************************
*                                                                                                                      *
* libscopehal                                                                                                          *
*                                                                                                                      *
* Copyright (c) 2012-2024 Andrew D. Zonenberg and contributors                                                         *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

/**
	@file
	@author Frederic Borry
	@brief Declaration of TinySA

	@ingroup scopedrivers
 */

#ifndef TinySA_h
#define TinySA_h

/**
	@brief Driver for TinySA and TinySA Ultra Spectrum Analizers
	@ingroup scopedrivers

	TinySA and TinySA Ultra are hobyist low-cost Spectrum Analizer designed by Erik Kaashoek: https://tinysa.org/
	They can be connected to a PC via a USB COM port.

 */
class TinySA
	: public virtual SCPISA
{
public:
	TinySA(SCPITransport* transport);
	virtual ~TinySA();

	//not copyable or assignable
	TinySA(const TinySA& rhs) =delete;
	TinySA& operator=(const TinySA& rhs) =delete;


public:

	//Channel configuration

	//Data acquisition
	virtual bool AcquireData() override;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Spectrum analyzer configuration

	virtual std::vector<uint64_t> GetSampleDepthsNonInterleaved() override;
	virtual void SetSpan(int64_t span) override;
	virtual int64_t GetSpan() override;
	virtual void SetCenterFrequency(size_t channel, int64_t freq) override;
	virtual int64_t GetCenterFrequency(size_t channel) override;

	virtual void SetResolutionBandwidth(int64_t rbw) override;

protected:
	enum Model {
		TINY_SA,
		TINY_SA_ULTRA
	};

	// Make sure several request don't collide before we received the corresponding response
	std::recursive_mutex m_transportMutex;

	std::string ConverseSingle(const std::string commandString);
	size_t ConverseMultiple(const std::string commandString, std::vector<std::string> &readLines);
	std::string ConverseString(const std::string commandString);
	size_t ConverseBinary(const std::string commandString, std::vector<uint8_t> &data, size_t length);
	int64_t ConverseRbwValue(bool sendValue = false, int64_t value = 0);
	bool ConverseSweep(int64_t &sweepStart, int64_t &sweepStop, bool setValue = false);

	static void RemoveCR(std::string &toClean)
	{
		toClean.erase( std::remove(toClean.begin(), toClean.end(), '\r'), toClean.end() );
	}

	std::string GetChannelColor(size_t i);

	int64_t m_rbwMin;
	int64_t m_rbwMax;

	Model m_tinySAModel;
	// Span control
	int64_t m_sweepStart;
	int64_t m_sweepStop;

	int64_t m_freqMin;
	int64_t m_freqMax;
	// dbm offset to apply on values received from the device (model depedant)
	int64_t m_modelDbmOffset;

	inline static const std::string TRAILER_STRING = "ch> ";
	inline static const size_t TRAILER_STRING_LENGTH = TRAILER_STRING.size();
	inline static const std::string EOL_STRING = "\r\n";
	inline static const size_t EOL_STRING_LENGTH = EOL_STRING.size();
	static const size_t MAX_RESPONSE_SIZE = 100*1024;
	inline static const double COMMUNICATION_TIMEOUT = 30; // 30s has a sweep with low rbw can take several minutes and we may have to wait that long between each data reception

public:
	static std::string GetDriverNameInternal();
	OSCILLOSCOPE_INITPROC(TinySA)
};

#endif
