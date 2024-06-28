using UnityEngine;
using UnityEngine.Events;

// ステージ画面のイントロ シーケンスを制御します。
public class StageIntroUI : MonoBehaviour
{
    // ステージが開始されたときに発生する UnityEvent
    public UnityEvent onPlayGame;

    // StageIntro アニメーション内のイベントから呼び出されます。
    public void OnPlayGameEvent()
    {
        onPlayGame.Invoke();
    }
}
