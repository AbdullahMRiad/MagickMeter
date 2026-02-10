#include "MagickMeter.h"

const std::map<LPCWSTR, Magick::CompositeOperator> compOperationMap{
    {L"ALPHACOMP",          Magick::AlphaComposite},
    {L"ATOP",               Magick::AtopComposite},
    {L"BLEND",              Magick::BlendComposite},
    {L"BLURCOMP",           Magick::BlurComposite},
    {L"BUMPMAP",            Magick::BumpmapComposite},
    {L"CHANGEMASK",         Magick::ChangeMaskComposite},
    {L"CLEAR",              Magick::ClearComposite},
    {L"COLORBURN",          Magick::ColorBurnComposite},
    {L"COLORDODGE",         Magick::ColorDodgeComposite},
    {L"COLORIZE",           Magick::ColorizeComposite},
    {L"COPYBLACK",          Magick::CopyBlackComposite},
    {L"COPYBLUE",           Magick::CopyBlueComposite},
    {L"COPY",               Magick::CopyComposite},
    {L"COPYCYAN",           Magick::CopyCyanComposite},
    {L"COPYGREEN",          Magick::CopyGreenComposite},
    {L"COPYMAGENTA",        Magick::CopyMagentaComposite},
    {L"COPYALPHA",          Magick::CopyAlphaComposite},
    {L"COPYRED",            Magick::CopyRedComposite},
    {L"COPYYELLOW",         Magick::CopyYellowComposite},
    {L"DARKEN",             Magick::DarkenComposite},
    {L"DARKENINTENSITY",    Magick::DarkenIntensityComposite},
    {L"DIFFERENCE",         Magick::DifferenceComposite},
    {L"DISPLACE",           Magick::DisplaceComposite},
    {L"DISSOLVE",           Magick::DissolveComposite},
    {L"DISTORT",            Magick::DistortComposite},
    {L"DIVIDEDST",          Magick::DivideDstComposite},
    {L"DIVIDESRC",          Magick::DivideSrcComposite},
    {L"DSTATOP",            Magick::DstAtopComposite},
    {L"DSTCOMP",            Magick::DstComposite},
    {L"DSTINCOMP",          Magick::DstInComposite},
    {L"DSTOUTCOMP",         Magick::DstOutComposite},
    {L"DSTOVERCOMP",        Magick::DstOverComposite},
    {L"EXCLUSION",          Magick::ExclusionComposite},
    {L"HARDLIGHT",          Magick::HardLightComposite},
    {L"HARDMIX",            Magick::HardMixComposite},
    {L"HUE",                Magick::HueComposite},
    {L"INCOMP",             Magick::InComposite},
    {L"INTENSITY",          Magick::IntensityComposite},
    {L"LIGHTEN",            Magick::LightenComposite},
    {L"LIGHTENINTENSITY",   Magick::LightenIntensityComposite},
    {L"LINEARBURN",         Magick::LinearBurnComposite},
    {L"LINEARDODGE",        Magick::LinearDodgeComposite},
    {L"LINEARLIGHT",        Magick::LinearLightComposite},
    {L"LUMINIZE",           Magick::LuminizeComposite},
    {L"MATHEMATICS",        Magick::MathematicsComposite},
    {L"MINUSDST",           Magick::MinusDstComposite},
    {L"MINUSSRC",           Magick::MinusSrcComposite},
    {L"MODULATE",           Magick::ModulateComposite},
    {L"MODULUSADD",         Magick::ModulusAddComposite},
    {L"MODULUSSUBTRACT",    Magick::ModulusSubtractComposite},
    {L"MULTIPLY",           Magick::MultiplyComposite},
    {L"NOCOMP",             Magick::NoComposite},
    {L"OUTCOMP",            Magick::OutComposite},
    {L"OVERCOMP",           Magick::OverComposite},
    {L"OVERLAY",            Magick::OverlayComposite},
    {L"PEGTOPLIGHT",        Magick::PegtopLightComposite},
    {L"PINLIGHT",           Magick::PinLightComposite},
    {L"PLUS",               Magick::PlusComposite},
    {L"REPLACE",            Magick::ReplaceComposite},
    {L"SATURATE",           Magick::SaturateComposite},
    {L"SCREEN",             Magick::ScreenComposite},
    {L"SOFTLIGHT",          Magick::SoftLightComposite},
    {L"SRCATOP",            Magick::SrcAtopComposite},
    {L"SRCCOMP",            Magick::SrcComposite},
    {L"SRCINCOMP",          Magick::SrcInComposite},
    {L"SRCOUTCOMP",         Magick::SrcOutComposite},
    {L"SRCOVERCOMP",        Magick::SrcOverComposite},
    {L"THRESHOLD",          Magick::ThresholdComposite},
    {L"VIVIDLIGHT",         Magick::VividLightComposite},
    {L"XOR",                Magick::XorComposite},
    {L"STEREO",             Magick::CompositeOperator::StereoComposite},
    {L"FREEZE",             Magick::FreezeComposite},
    {L"INTERPOLATE",        Magick::InterpolateComposite},
    {L"NEGATE",             Magick::NegateComposite},
    {L"REFLECT",            Magick::ReflectComposite},
    {L"SOFTBURN",           Magick::SoftBurnComposite},
    {L"SOFTDODGE",          Magick::SoftDodgeComposite},
    {L"STAMP",              Magick::StampComposite},
    {L"RMSE",               Magick::RMSEComposite},
    {L"SALIENCYBLEND",      Magick::SaliencyBlendComposite},
    {L"SEAMLESSBLEND",      Magick::SeamlessBlendComposite}
};

BOOL Measure::CreateCombine(std::shared_ptr<ImgContainer> out)
{
    const int baseIndex = Utils::NameToIndex(out->config.at(0).para);
    if (baseIndex != -1 && baseIndex < imgList.size())
    {
        out->img = imgList.at(baseIndex)->img;
        imgList.at(baseIndex)->isCombined = TRUE;
    }
    else
    {
        RmLogF(rm, 2, L"Combine: %s is invalid.", out->config.at(0).para);
        return FALSE;
    }

    for (auto &option : out->config)
    {
        if (option.isApplied)
            continue;

        auto compOp = std::find_if(
            compOperationMap.begin(),
            compOperationMap.end(),
            [option](const std::pair<LPCWSTR, Magick::CompositeOperator> &i) {
                return Utils::IsEqual(option.name, i.first);
            }
        );

        if (compOp == compOperationMap.end())
        {
            continue;
        }

        // Found valid Composite operation
        option.isApplied = TRUE;

        const int index = Utils::NameToIndex(option.para);

        if (index < 0 || index >= imgList.size())
        {
            RmLogF(rm, 2, L"Combine: %s is invalid.", option.para);
            return FALSE;
        }

        ImgContainer* childImage = &*imgList.at(index);

        childImage->isCombined = TRUE;

        //Extend main image size
        const size_t newW = max(
            childImage->img.columns(),
            out->img.columns()
        );
        const size_t newH = max(
            childImage->img.rows(),
            out->img.rows()
        );

        Magick::Geometry newSize(newW, newH);

        try
        {
            Magick::Image temp = Magick::Image(out->img);

            out->img = Magick::Image(newSize, INVISIBLE);
            out->img.composite(temp, 0, 0, Magick::OverComposite);

            temp = Magick::Image(newSize, INVISIBLE);
            temp.composite(childImage->img, 0, 0, Magick::OverComposite);

            out->img.composite(temp, 0, 0, compOp->second);
        }
        catch (Magick::Exception &error_)
        {
            LogError(error_);
            return FALSE;
        }
    }

    return TRUE;
}
