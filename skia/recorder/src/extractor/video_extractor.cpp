#include "extractor/video_extractor.hpp"

VideoExtractor::VideoExtractor(const std::string& path,
                               const std::string& artboardName,
                               const std::string& animationName,
                               const std::string& watermark,
                               const std::string& destination,
                               int width,
                               int height,
                               int smallExtentTarget,
                               int maxWidth,
                               int maxHeight,
                               int duration,
                               int minDuration,
                               int maxDuration,
                               int fps,
                               int bitrate) :
    RiveFrameExtractor(path,
                       artboardName,
                       animationName,
                       watermark,
                       destination,
                       width,
                       height,
                       smallExtentTarget,
                       maxWidth,
                       maxHeight,
                       duration,
                       minDuration,
                       maxDuration,
                       fps),
    m_movieWriter(
        new MovieWriter(destination, m_Width, m_Height, m_Fps, bitrate))
{
	// TODO: set the properties directly on the animation?
	// m_Animation->fps(m_Fps);
	// m_Animation->duration(m_Duration);
}

VideoExtractor::~VideoExtractor()
{
	if (m_movieWriter)
	{
		delete m_movieWriter;
	}
}

void VideoExtractor::extractFrames(int numLoops) const
{
	m_movieWriter->writeHeader();
	RiveFrameExtractor::extractFrames(numLoops);
	m_movieWriter->finalize();
}

void VideoExtractor::onNextFrame(int frameNumber) const
{
	auto pixelData = this->getPixelAddresses();
	m_movieWriter->writeFrame(frameNumber, (const uint8_t* const*)&pixelData);
}