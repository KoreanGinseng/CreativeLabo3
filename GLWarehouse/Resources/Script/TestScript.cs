using System;
using System.Collections;
using System.Collections.Generic;
using Sample;

namespace Sample
{
	public class TestScript : ScriptBase
    {
        float speed = 1;

        /// <summary>
        /// 継承先で実装する開始メソッド
        /// </summary>
        override protected void Start()
        {
        }

        /// <summary>
        /// 継承先で実装する更新メソッド
        /// </summary>
        override protected void Update()
        {
            X += speed;
            if (X <= 0 || X >= 900)
            {
                speed *= -1;
            }
        }
    }
}
