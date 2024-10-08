/***********************************************************************************************************************
*                                                                                                                      *
* libscopeprotocols                                                                                                    *
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
	@author Andrew D. Zonenberg
	@brief Declaration of PcapngImportFilter
 */
#ifndef PcapngImportFilter_h
#define PcapngImportFilter_h

class PcapngImportFilter : public PacketDecoder
{
public:
	PcapngImportFilter(const std::string& color);

	std::vector<std::string> GetHeaders() override;
	virtual void SetDefaultName() override;
	virtual void Refresh(vk::raii::CommandBuffer& cmdBuf, std::shared_ptr<QueueHandle> queue) override;
	virtual bool ValidateChannel(size_t i, StreamDescriptor stream) override;

	static std::string GetProtocolName();

	PROTOCOL_DECODER_INITPROC(PcapngImportFilter)

protected:
	std::string m_fpname;
	std::string m_datarate;

	void OnFileNameChanged();

	bool ValidateSHB(FILE* fp);
	bool ReadIDB(FILE* fp);
	std::string ReadFixedLengthString(uint16_t len, FILE* fp);

	bool LoadLinuxCooked(FILE* fp);
	bool LoadCANLinuxCooked(FILE* fp);
	bool LoadSocketCAN(FILE* fp);

	enum LinkType
	{
		LINK_TYPE_ETHERNET,
		LINK_TYPE_CAN,
		LINK_TYPE_USB,
		LINK_TYPE_LINUX_COOKED,
		LINK_TYPE_SOCKETCAN,
		LINK_TYPE_UNKNOWN
	} m_linkType;

	int64_t m_timestampScale;
};

#endif
