using System;
using Sample;

namespace Sample
{
	public class ScriptBase : IScript
    {
        /// <summary>
        /// C++との連携用ラッパー
        /// </summary>
        SharedWrapper wrapper;

        //プロパティとしてのアクセス
        protected float X { get { return wrapper.X(); } set { wrapper.X(value); } }
        protected float Y { get { return wrapper.Y(); } set { wrapper.Y(value); } }
        protected float Z { get { return wrapper.Z(); } set { wrapper.Z(value); } }

        /// <summary>
        /// C++から呼び出されるメソッド
        /// </summary>
        /// <param name="sharedData">共有用データ</param>
        public bool CallStart(SharedWrapper sharedData)
        {
            wrapper = sharedData;
            Start();
            return true;
        }

        /// <summary>
        /// C++から呼び出されるメソッド
        /// </summary>
        /// <param name="sharedData">共有用データ</param>
        public bool CallUpdate(SharedWrapper sharedData)
        {
            wrapper = sharedData;
            Update();
            return true;
        }

        /// <summary>
        /// 継承先で実装する開始メソッド
        /// </summary>
        virtual protected void Start()
        {
        }

        /// <summary>
        /// 継承先で実装する更新メソッド
        /// </summary>
        virtual protected void Update()
        {
        }
	}
}
