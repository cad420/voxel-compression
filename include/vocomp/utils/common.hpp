#pragma once

#include <VMUtils/fmt.hpp>
#include <VMUtils/concepts.hpp>
#include <VMUtils/attributes.hpp>
#include <VMUtils/modules.hpp>
#include "io.hpp"

VM_BEGIN_MODULE( vol )

using namespace std;

#pragma pack( push )
#pragma pack( 4 )

VM_EXPORT
{
	enum class EncodeMethod : uint64_t
	{
		H264,
		HEVC
	};

	enum class EncodePreset : uint32_t
	{
		Default,
		HP,
		HQ,
		BD,
		LowLatencyDefault,
		LowLatencyHQ,
		LowLatencyHP,
		LosslessDefault,
		LosslessHP
	};
	enum class PixelFormat : uint32_t
	{
		IYUV,
		YV12,
		NV12,
		YUV42010Bit,
		YUV444,
		YUV44410Bit,
		ARGB,
		ARGB10,
		AYUV,
		ABGR,
		ABGR10
	};
	enum class CompressDevice : uint32_t
	{
		Cuda,	/* cuda sdk required */
		Graphics /* D3D9 for windows and GL for linux */
	};

	struct VideoCompressOptions
	{
		VideoCompressOptions();

		VM_DEFINE_ATTRIBUTE( CompressDevice, device );
		VM_DEFINE_ATTRIBUTE( EncodeMethod, encode_method ) = EncodeMethod::H264;
		VM_DEFINE_ATTRIBUTE( EncodePreset, encode_preset ) = EncodePreset::Default;
		VM_DEFINE_ATTRIBUTE( unsigned, width ) = 1024;
		VM_DEFINE_ATTRIBUTE( unsigned, height ) = 1024;
		VM_DEFINE_ATTRIBUTE( unsigned, batch_frames ) = 64;
		VM_DEFINE_ATTRIBUTE( PixelFormat, pixel_format ) = PixelFormat::IYUV;
	};

	struct BlockIndex
	{
		VM_DEFINE_ATTRIBUTE( uint32_t, first_frame );
		VM_DEFINE_ATTRIBUTE( uint32_t, last_frame );
		VM_DEFINE_ATTRIBUTE( uint64_t, offset );

		bool operator<( BlockIndex const &other ) const
		{
			return first_frame < other.first_frame ||
				   first_frame == other.first_frame && ( offset < other.offset );
		}

		bool operator==( BlockIndex const &other ) const
		{
			return first_frame == other.first_frame &&
				   last_frame == other.last_frame &&
				   offset == other.offset;
		}

		friend ostream &operator<<( ostream &os, BlockIndex const &_ )
		{
			vm::fprint( os, "{{ f0: {}, f1: {}, offset:{} }}", _.first_frame, _.last_frame, _.offset );
			return os;
		}
	};

	struct Idx
	{
		VM_DEFINE_ATTRIBUTE( uint32_t, x );
		VM_DEFINE_ATTRIBUTE( uint32_t, y );
		VM_DEFINE_ATTRIBUTE( uint32_t, z );

		uint64_t total() const { return (uint64_t)x * y * z; }

		bool operator<( Idx const &other ) const
		{
			return x < other.x ||
				   x == other.x && ( y < other.y ||
									 y == other.y && z < other.z );
		}
		bool operator==( Idx const &other ) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		friend ostream &operator<<( ostream &os, Idx const &_ )
		{
			vm::fprint( os, "{}", make_tuple( _.x, _.y, _.z ) );
			return os;
		}
	};
}

struct Header
{
	VM_DEFINE_ATTRIBUTE( uint64_t, version );
	VM_DEFINE_ATTRIBUTE( Idx, raw );
	VM_DEFINE_ATTRIBUTE( Idx, dim );
	VM_DEFINE_ATTRIBUTE( Idx, adjusted );
	VM_DEFINE_ATTRIBUTE( uint64_t, log_block_size );
	VM_DEFINE_ATTRIBUTE( uint64_t, block_size );
	VM_DEFINE_ATTRIBUTE( uint64_t, block_inner );
	VM_DEFINE_ATTRIBUTE( uint64_t, padding );
	VM_DEFINE_ATTRIBUTE( EncodeMethod, encode_method );
	VM_DEFINE_ATTRIBUTE( uint64_t, frame_size );

	friend std::ostream &operator<<( std::ostream &os, Header const &header )
	{
		vm::fprint( os, "version: {}\nraw: {}\ndim: {}\nadjusted: {}\n"
						"log_block_size: {}\nblock_size: {}\nblock_inner: {}\n"
						"padding: {}\nframe_size: {}",
					header.version,
					header.raw,
					header.dim,
					header.adjusted,
					header.log_block_size,
					header.block_size,
					header.block_inner,
					header.padding,
					header.frame_size );
		return os;
	}
};

#pragma pack( pop )

VM_END_MODULE()
