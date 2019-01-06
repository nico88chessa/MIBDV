#ifndef GET_MODE_PARAMETER_MARSHALLER_HPP
#define GET_MODE_PARAMETER_MARSHALLER_HPP

#include "../beans/GetModeParameter.hpp"
#include "../marshalling/IpgMarshaller.hpp"

namespace ipg {

class GetModeParameterMarshaller : public IpgMarshaller<GetModeParameterInput, GetModeParameterOutput> {
public:
    typedef GetModeParameterMarshaller* Ptr;
    typedef const GetModeParameterMarshaller* ConstPtr;

protected:

    virtual bool marshallData(GetModeParameterInput::ConstPtr input, QByteArray& bytes) {

        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << input->getModeIndex();

//        bytes.append(input->getModeIndex());

        return true;

    }

    virtual bool unmarshallData(const QByteArray& bytes, GetModeParameterOutput::Ptr output) {

        const int offsets[11] = {
            0,
            2,
            4,
            8,
            12,
            16,
            20,
            24,
            28,
            32,
            160
        };

        IPG_USHORT version;
        IPG_USHORT modeIndex;
        IPG_FLOAT nominalFrequency;
        IPG_FLOAT nominalPulseEnergy;
        IPG_FLOAT nominalPulseDuration;
        IPG_FLOAT nominalAveragePower;
        IPG_FLOAT nominalPeakPower;
        IPG_FLOAT minimumFrequency;
        IPG_FLOAT maximumFrequency;
        QString modeDescription;
        int size = 0;

        QDataStream stream(bytes);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        stream >> version;
        stream >> modeIndex;
        stream >> nominalFrequency;
        stream >> nominalPulseEnergy;
        stream >> nominalPulseDuration;
        stream >> nominalAveragePower;
        stream >> nominalPeakPower;
        stream >> minimumFrequency;
        stream >> maximumFrequency;

        size = offsets[10] - offsets[9];
        char* modeDescriptionBuffer = new char[size];
        stream.readRawData(modeDescriptionBuffer, size);
        modeDescription.append(modeDescriptionBuffer);

        output->setVersion(version);
        output->setModeIndex(modeIndex);
        output->setNominalFrequency(nominalFrequency);
        output->setNominalPulseEnergy(nominalPulseEnergy);
        output->setNominalPulseDuration(nominalPulseDuration);
        output->setNominalAveragePower(nominalAveragePower);
        output->setNominalPeakPower(nominalPeakPower);
        output->setMinimumFrequency(minimumFrequency);
        output->setMaximumFrequency(maximumFrequency);
        output->setModeDescription(modeDescription);

        /*
        bool ok;

        version = bytes.mid(offsets[0], offsets[1] - offsets[0]).toUShort(&ok);
        if (!ok) return false;

        modeIndex = bytes.mid(offsets[1], offsets[2] - offsets[1]).toUShort(&ok);
        if (!ok) return false;

        nominalFrequency = bytes.mid(offsets[2], offsets[3] - offsets[2]).toFloat(&ok);
        if (!ok) return false;

        nominalPulseEnergy = bytes.mid(offsets[3], offsets[4] - offsets[3]).toFloat(&ok);
        if (!ok) return false;

        nominalPulseDuration = bytes.mid(offsets[4], offsets[5] - offsets[4]).toFloat(&ok);
        if (!ok) return false;

        nominalAveragePower = bytes.mid(offsets[5], offsets[6] - offsets[5]).toFloat(&ok);
        if (!ok) return false;

        nominalPeakPower = bytes.mid(offsets[6], offsets[7] - offsets[6]).toFloat(&ok);
        if (!ok) return false;

        minimumFrequency = bytes.mid(offsets[7], offsets[8] - offsets[7]).toFloat(&ok);
        if (!ok) return false;

        maximumFrequency = bytes.mid(offsets[8], offsets[9] - offsets[8]).toFloat(&ok);
        if (!ok) return false;

        modeDescription < bytes.mid(offsets[9], offsets[10] - offsets[9]);

        output->setVersion(version);
        output->setModeIndex(modeIndex);
        output->setNominalFrequency(nominalFrequency);
        output->setNominalPulseEnergy(nominalPulseEnergy);
        output->setNominalPulseDuration(nominalPulseDuration);
        output->setNominalAveragePower(nominalAveragePower);
        output->setNominalPeakPower(nominalPeakPower);
        output->setMinimumFrequency(minimumFrequency);
        output->setMaximumFrequency(maximumFrequency);
        output->setModeDescription(modeDescription);

        */
        delete[] modeDescriptionBuffer;

        return true;

    }

};


}

#endif // GET_MODE_PARAMETER_MARSHALLER_HPP
