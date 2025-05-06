#include "SeqAnimation.h"
#include "tinyxml.h"
#include "iostream"
#include "TextureManager.h"

SeqAnimation::SeqAnimation(bool repeat) : Animation(repeat) {}

void SeqAnimation::DrawFrame(float x, float y, float xScale, float yScale, SDL_RendererFlip flip) {
    TextureManager::GetInstance()->Draw(m_CurrentSeq.TextureIDs[m_CurrentFrame], x, y, m_CurrentSeq.Width, m_CurrentSeq.Height, xScale, yScale, flip);
}

void SeqAnimation::Update(float dt) {
    if (m_Repeat || !m_IsEnded) {
        m_IsEnded = false;
        m_CurrentFrame = (SDL_GetTicks() / m_CurrentSeq.Speed) % m_CurrentSeq.FrameCount;
    }

    if (!m_Repeat && m_CurrentFrame == (m_CurrentSeq.FrameCount - 1)) {
        m_IsEnded = true;
       // m_CurrentFrame = m_SegMap[m_CurrentSeq.FrameCount - 1];
       if (m_SegMap.find(std::to_string(m_CurrentSeq.FrameCount - 1)) != m_SegMap.end()) {
   // m_CurrentFrame = m_SegMap[std::to_string(m_CurrentSeq.FrameCount - 1)];
} else {
    std::cout << "Không tìm thấy key trong m_SegMap!" << std::endl;
}

    }
}

void SeqAnimation::SetCurrentSeq(std::string seqID) {
    if (m_SegMap.count(seqID) > 0) {
        m_CurrentSeq = m_SegMap[seqID];
    } else {
        std::cout << "The given Sequence animation is not matching: " << seqID << std::endl;
    }
}

void SeqAnimation::Parse(std::string source) {
    TiXmlDocument xml;
    xml.LoadFile(source);
    if (xml.Error()) {
        std::cout << "Failed to load animation file: " << source << std::endl;
        return;
    }

    TiXmlElement* root = xml.RootElement();
    for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        if (e->Value() == std::string("sequence")) {
            Sequence seq;
            std::string seqID = e->Attribute("id");
            e->Attribute("speed", &seq.Speed);
        e->Attribute("width", &seq.Width);
        e->Attribute("height", &seq.Height);
        e->Attribute("frameCount", &seq.FrameCount);
        for (TiXmlElement* frame = e->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement()) {
            seq.TextureIDs.push_back(frame->Attribute("textureID"));
        }
        m_SegMap[seqID] = seq;
        }
    }
}
